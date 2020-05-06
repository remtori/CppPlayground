#pragma once

#include "Assertions.h"
#include "StdLibExtras.h"
#include "Traits.h"
#include "TypedTransfer.h"
#include "Types.h"
#include <initializer_list>

namespace ASL {

template<typename T>
class Vector {
public:
    Vector() {}

    Vector(size_t reserved_size)
    {
        ensure_capacity(reserved_size);
    }

    Vector(std::initializer_list<T> list)
    {
        ensure_capacity(list.size());
        for (auto& v : list)
            unchecked_append(v);
    }

    Vector(const Vector& other)
    {
        ensure_capacity(other.m_size);
        TypedTransfer<T>::copy(data(), other.data(), other.size());
        m_size = other.m_size;
    }

    Vector& operator=(const Vector& other)
    {
        if (this != &other) {
            clear();
            ensure_capacity(other.m_size);
            TypedTransfer<T>::copy(data(), other.data(), other.size());
            m_size = other.m_size;
        }
        return *this;
    }

    Vector(Vector&& other)
        : m_buffer(other.m_buffer)
        , m_size(other.m_size)
        , m_capacity(other.m_capacity)
    {
        other.m_buffer = nullptr;
        other.m_size = 0;
        other.m_capacity = 0;
    }

    Vector& operator=(Vector&& other)
    {
        if (this != &other) {
            clear();
            m_size = other.m_size;
            m_capacity = other.m_capacity;
            m_buffer = other.m_buffer;

            other.m_buffer = nullptr;
            other.m_size = 0;
            other.m_capacity = 0;
        }

        return *this;
    }

    ~Vector()
    {
        clear();
    }

    inline bool is_empty() const { return m_size == 0; }
    inline size_t size() const { return m_size; }
    inline size_t capacity() const { return m_capacity; }

    T* data() { return m_buffer; }
    const T* data() const { return m_buffer; }

    inline T& at(size_t i)
    {
        ASSERT(i >= 0 && i < m_size);
        return m_buffer[i];
    }

    inline const T& at(size_t i) const
    {
        ASSERT(i >= 0 && i < m_size);
        return m_buffer[i];
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

    void append(const Vector& other)
    {
        grow_capacity(m_size + other.m_size);
        TypedTransfer<T>::copy(data() + m_size, other.data(), other.size());
        m_size += other.m_size;
    }

    void append(Vector&& other)
    {
        if (is_empty()) {
            *this = move(other);
            return;
        }

        auto other_size = other.m_size;
        Vector tmp = move(other);
        grow_capacity(m_size + other_size);
        TypedTransfer<T>::move(data() + m_size, tmp.data(), other_size);
        m_size += other_size;
    }

    void prepend(T&& value)
    {
        insert(0, move(value));
    }

    void prepend(const T& value)
    {
        insert(0, value);
    }

    void prepend(Vector&& other)
    {
        if (other.is_empty())
            return;

        if (is_empty()) {
            *this = move(other);
            return;
        }

        auto other_size = other.m_size;
        grow_capacity(m_size + other_size);

        for (size_t i = m_size + other_size - 1; i >= other_size; --i) {
            new (&m_buffer[i]) T(move(at(i - other_size)));
            at(i - other_size).~T();
        }

        Vector tmp = move(other);
        TypedTransfer<T>::move(m_buffer, tmp.data(), other_size);
        m_size += other_size;
    }

    void insert(size_t index, T&& value)
    {
        ASSERT(index <= m_size);
        if (index == m_size)
            return append(move(value));

        grow_capacity(m_size + 1);

        ++m_size;
        if constexpr (Traits<T>::is_trivial()) {
            TypedTransfer<T>::move(&m_buffer[index + 1], &m_buffer[index], m_size - index - 1);
        } else {
            for (size_t i = m_size - 1; i > index; --i) {
                new (&m_buffer[i]) T(move(at(i - 1)));
                at(i - 1).~T();
            }
        }

        new (&m_buffer[index]) T(move(value));
    }

    void insert(size_t index, const T& value)
    {
        insert(index, T(value));
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
            TypedTransfer<T>::move(&m_buffer[index], &m_buffer[index + 1], m_size - index - 1);
        } else {
            at(index).~T();
            for (size_t i = index + 1; i < m_size; ++i) {
                new (&at(i - 1)) T(move(at(i)));
                at(i).~T();
            }
        }

        --m_size;
    }

    void grow_capacity(size_t needed_capacity)
    {
        if (m_capacity >= needed_capacity)
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
            TypedTransfer<T>::move(new_buffer, m_buffer, m_size);
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

    bool operator==(const Vector& other) const
    {
        if (m_size != other.m_size)
            return false;

        return TypedTransfer<T>::compare(data(), other.data(), size());
    }

    bool operator!=(const Vector& other) const
    {
        return !(*this == other);
    }

private:
    T* m_buffer = nullptr;
    size_t m_size = 0;
    size_t m_capacity = 0;
};

} // namespace ASL

using ASL::Vector;
