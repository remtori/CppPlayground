#pragma once

#include <ASL/Assertions.h>
#include <ASL/StdLibExtras.h>
#include <ASL/Types.h>

namespace ASL {

template<typename T>
class alignas(T) Optional {
public:
    Optional() {}

    Optional(const T& v)
        : m_has_value(true)
    {
        new (&m_storage) T(v);
    }

    template<typename U>
    Optional(const U& v)
        : m_has_value(true)
    {
        new (&m_storage) T(v);
    }

    Optional(const T&& v)
        : m_has_value(true)
    {
        new (&m_storage) T(move(v));
    }

    Optional(const Optional& other)
        : m_has_value(other.m_has_value)
    {
        if (m_has_value) {
            new (&m_storage) T(other.value());
        }
    }

    Optional& operator=(const Optional& other)
    {
        if (this != &other) {
            clear();
            m_has_value = other.m_has_value;
            if (m_has_value) {
                new (&m_storage) T(other.value());
            }
        }
        return *this;
    }

    Optional(Optional&& other)
        : m_has_value(other.m_has_value)
    {
        if (other.has_value()) {
            new (&m_storage) T(other.release_value());
            other.m_has_value = false;
        }
    }

    Optional& operator=(Optional&& other)
    {
        if (this != &other) {
            clear();
            m_has_value = other.m_has_value;
            if (other.has_value())
                new (&m_storage) T(other.release_value());
        }
        return *this;
    }

    ~Optional()
    {
        clear();
    }

    void clear()
    {
        if (m_has_value) {
            value().~T();
            m_has_value = false;
        }
    }

    bool has_value() const { return m_has_value; }

    T& value()
    {
        ASSERT(m_has_value);
        return *reinterpret_cast<T*>(&m_storage);
    }

    const T& value() const
    {
        return value();
    }

    T release_value()
    {
        ASSERT(m_has_value);
        T released_value = move(value());
        value().~T();
        m_has_value = false;
        return released_value;
    }

    T value_or(const T& fallback)
    {
        if (m_has_value)
            return value();

        return fallback;
    }

private:
    bool m_has_value = false;
    u8 m_storage[sizeof(T)] = { 0 };
};

} // namespace ASL

using ASL::Optional;
