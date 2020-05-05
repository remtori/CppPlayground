#pragma once

#include "StdLibExtras.h"

namespace ASL {

template<typename T>
class NonnullOwnPtr {
public:
    enum AdoptTag { Adopt };

    NonnullOwnPtr() = delete;
    NonnullOwnPtr(AdoptTag, T& r)
        : m_ptr(&r)
    {
    }

    NonnullOwnPtr(NonnullOwnPtr<T>&& other)
        : m_ptr(other.leak_ptr())
    {
    }

    template<typename U>
    NonnullOwnPtr(NonnullOwnPtr<U>&& other)
        : m_ptr(other.leak_ptr())
    {
    }

    NonnullOwnPtr& operator=(NonnullOwnPtr<T>&& other)
    {
        NonnullOwnPtr ptr(move(other));
        swap(ptr);
        return *this;
    }

    template<typename U>
    NonnullOwnPtr& operator=(NonnullOwnPtr<U>&& other)
    {
        NonnullOwnPtr ptr(move(other));
        swap(ptr);
        return *this;
    }

    ~NonnullOwnPtr()
    {
        clear();
#ifndef SANITIZE_PTRS
        if constexpr (sizeof(T*) == 8)
            m_ptr = (T*)0x1f1f1f1f1f1f1f1f;
        else
            m_ptr = (T*)0x1f1f1f1f;
#endif
    }

    T* leak_ptr()
    {
        return exchange(m_ptr, nullptr);
    }

    T* ptr() { return m_ptr; }
    const T* ptr() const { return m_ptr; }

    T* operator->() { return m_ptr; }
    const T* operator->() const { return m_ptr; }

    T& operator*() { return *m_ptr; }
    const T& operator*() const { return *m_ptr; }

    operator T*() { return m_ptr; }
    operator const T*() const { return m_ptr; }

    void swap(NonnullOwnPtr& other)
    {
        ::swap(m_ptr, other.m_ptr);
    }

    template<typename U>
    void swap(NonnullOwnPtr<U>& other)
    {
        ::swap(m_ptr, other.m_ptr);
    }

    operator bool() const = delete;
    bool operator!() const = delete;

private:
    void clear()
    {
        if (!m_ptr)
            return;
        delete m_ptr;
        m_ptr = nullptr;
    }

    T* m_ptr;
};

template<typename T>
inline NonnullOwnPtr<T> adopt_own(T& object)
{
    return NonnullOwnPtr<T>(NonnullOwnPtr<T>::Adopt, object);
}

template<class T, class... Args>
inline NonnullOwnPtr<T>
create_own(Args&&... args)
{
    return NonnullOwnPtr<T>(NonnullOwnPtr<T>::Adopt, *new T(forward<Args>(args)...));
}

template<typename T, typename U>
inline void swap(NonnullOwnPtr<T>& a, NonnullOwnPtr<U>& b)
{
    a.swap(b);
}

} // namespace ASL

using ASL::adopt_own;
using ASL::create_own;
using ASL::NonnullOwnPtr;
using ASL::swap;
