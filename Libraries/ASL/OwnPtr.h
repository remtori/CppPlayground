#pragma once

#include "Assertions.h"
#include "NonnullOwnPtr.h"

namespace ASL {

template<typename T>
class OwnPtr {
public:
    OwnPtr() {}

    explicit OwnPtr(T* ptr)
        : m_ptr(ptr)
    {
    }

    OwnPtr(OwnPtr<T>&& other)
        : m_ptr(other.leak_ptr())
    {
    }

    template<typename U>
    OwnPtr(OwnPtr<U>&& other)
        : m_ptr(other.leak_ptr())
    {
    }

    template<typename U>
    OwnPtr(NonnullOwnPtr<U>&& other)
        : m_ptr(other.leak_ptr())
    {
    }

    OwnPtr(std::nullptr_t) {};

    OwnPtr& operator=(OwnPtr<T>&& other)
    {
        OwnPtr ptr(move(other));
        swap(ptr);
        return *this;
    }

    template<typename U>
    OwnPtr& operator=(OwnPtr<U>&& other)
    {
        OwnPtr ptr(move(other));
        swap(ptr);
        return *this;
    }

    template<typename U>
    OwnPtr& operator=(NonnullOwnPtr<U>&& other)
    {
        OwnPtr ptr(move(other));
        swap(ptr);
        ASSERT(m_ptr);
        return *this;
    }

    OwnPtr& operator=(T* ptr)
    {
        if (m_ptr != ptr)
            delete m_ptr;
        m_ptr = ptr;
        return *this;
    }

    OwnPtr& operator=(std::nullptr_t)
    {
        clear();
        return *this;
    }

    ~OwnPtr()
    {
        clear();
#ifndef SANITIZE_PTRS
        if constexpr (sizeof(T) == 8)
            m_ptr = (T*)0x1f1f1f1f1f1f1f1f;
        else
            m_ptr = (T*)0x1f1f1f1f;
#endif
    }

    bool has_ptr() const { return m_ptr != nullptr; }

    T* ptr() { return m_ptr; }
    const T* ptr() const { return m_ptr; }

    T* leak_ptr()
    {
        T* leaked_ptr = m_ptr;
        m_ptr = nullptr;
        return leaked_ptr;
    }

    NonnullOwnPtr<T> release_nonnull()
    {
        ASSERT(m_ptr);
        return NonnullOwnPtr<T>(NonnullOwnPtr<T>::Adopt, *leak_ptr());
    }

    template<typename U>
    NonnullOwnPtr<U> release_nonnull()
    {
        ASSERT(m_ptr);
        return NonnullOwnPtr<U>(NonnullOwnPtr<U>::Adopt, static_cast<U&>(*leak_ptr()));
    }

    T* operator->()
    {
        ASSERT(m_ptr);
        return m_ptr;
    }

    const T* operator->() const
    {
        ASSERT(m_ptr);
        return m_ptr;
    }

    T& operator*()
    {
        ASSERT(m_ptr);
        return *m_ptr;
    }

    const T& operator*() const
    {
        ASSERT(m_ptr);
        return *m_ptr;
    }

    void clear()
    {
        delete m_ptr;
        m_ptr = nullptr;
    }

    operator bool() const { return m_ptr != nullptr; }
    bool operator!() const { return m_ptr == nullptr; }

    void swap(OwnPtr& other)
    {
        ::swap(m_ptr, other.m_ptr);
    }

    template<typename U>
    void swap(OwnPtr<U>& other)
    {
        ::swap(m_ptr, other.m_ptr);
    }

private:
    T* m_ptr = nullptr;
};

template<typename T, typename U>
inline void swap(OwnPtr<T>& a, OwnPtr<U>& b)
{
    a.swap(b);
}

} // namespace ASL

using ASL::OwnPtr;
using ASL::swap;
