#pragma once

#include "Assertions.h"
#include "StdLibExtras.h"
#include "Traits.h"
#include "Types.h"
#include <stdlib.h> // For malloc and free

namespace ASL {

template<typename T>
class Vector {
public:
    Vector() {}

    inline bool is_empty() const { return m_size == 0; }
    inline size_t size() const { return m_size; }
    inline size_t capacity() const { return m_capacity; }

    T* data() { return m_buffer; }
    const T* data() const { return m_buffer; }

    inline T& at(size_t i)
    {
        ASSERT(i >= 0 && i < m_size);
        return data()[i];
    }

    inline const T& at(size_t i) const
    {
        ASSERT(i >= 0 && i < m_size);
        return data()[i];
    }

    inline T& operator[](size_t i) { return at(i); }
    inline const T& operator[](size_t i) const { return at(i); }

    T& front() { return at(0); }
    const T& front() const { return at(0); }

    T& back() { return at(size() - 1); }
    const T& back() const { return at(size() - 1); }

    T take_back()
    {
        ASSERT(!is_empty());
        T value = move(back());
        back().~T();
        --m_size;
        return value;
    }

    T take_front()
    {
        ASSERT(!is_empty());
        T value = move(front());
        remove(0);
        return value;
    }

    T take(size_t index)
    {
        T value = move(at(index));
        remove(index);
        return value;
    }

    template<class... Args>
    void emplace(Args&&... args)
    {
        grow_capacity(m_size + 1);
        new (&m_buffer(m_size)) T(forward<Args>(args)...);
        ++m_size;
    }

    inline void append(T&& value)
    {
        grow_capacity(m_size + 1);
        new (&m_buffer[m_size]) T(move(value));
        ++m_size;
    }

    inline void append(const T& value)
    {
        append(T(value));
    }

    void unchecked_append(T&& value)
    {
        ASSERT((m_size + 1) <= m_capacity);
        new (&m_buffer[m_size]) T(move(value));
        ++m_size;
    }

    void unchecked_append(const T& value)
    {
        unchecked_append(T(value));
    }

    void unstable_remove(size_t index)
    {
        ASSERT(index < m_size);
        swap(at(index), at(m_size - 1));
        take_back();
    }

    void remove(size_t index)
    {
        ASSERT(index < m_size);

        if constexpr (Traits<T>::is_trivial()) {
            __builtin_memcopy(m_buffer[index], m_buffer[index] + 1, m_size - index - 1);
        } else {
            at(index).~T();
            for (size_t i = index + 1; i < m_size; ++i) {
                new (m_buffer[i - 1]) T(move(at(i)));
                at(i).~T();
            }
        }

        --m_size;
    }

    void grow_capacity(size_t needed_capacity)
    {
        if (needed_capacity >= m_capacity)
            return;

        // grow with some padding to avoid repeated malloc and free
        ensure_capacity(max(static_cast<size_t>(4), needed_capacity + (needed_capacity / 4) + 4));
    }

    void ensure_capacity(size_t needed_capacity)
    {
        if (m_capacity >= needed_capacity)
            return;

        size_t new_capacity = needed_capacity;
        auto new_buffer = (T*)malloc(sizeof(T) * new_capacity);

        if constexpr (Traits<T>::is_trivial()) {
            __builtin_memmove(new_buffer, m_buffer, m_size * sizeof(T));
        } else {
            for (size_t i = 0; i < m_size; i++) {
                new (&new_buffer[i]) T(move(m_buffer[i]));
                at(i).~T();
            }
        }

        free(m_buffer);
        m_buffer = new_buffer;
        m_capacity = new_capacity;
    }

    void clear()
    {
        clear_with_capacity();
        if (m_buffer) {
            free(m_buffer);
            m_buffer = nullptr;
        }
        m_capacity = 0;
    }

    void clear_with_capacity()
    {
        for (size_t i = 0; i < m_size; ++i)
            m_buffer[i].~T();
        m_size = 0;
    }

private:
    T* m_buffer = nullptr;
    size_t m_size = 0;
    size_t m_capacity = 0;
};

} // namespace ASL

using ASL::Vector;
