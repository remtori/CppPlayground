#pragma once

#include "Assertions.h"
#include "StdLibExtras.h"

namespace ASL {

template<typename T>
inline void ref(T* ptr)
{
    if (ptr)
        ptr->ref();
}

template<typename T>
inline void unref(T* ptr)
{
    if (ptr)
        ptr->unref();
}

template<typename T>
class NonnullRefPtr {
public:
    enum AdoptTag { Adopt };

    NonnullRefPtr() = delete;

    NonnullRefPtr(const T& other)
        : m_ptr(const_cast<T*>(&other))
    {
        ref(m_ptr);
    }

    template<typename U>
    NonnullRefPtr(const U& other)
        : m_ptr(const_cast<U*>(&other))
    {
        ref(m_ptr);
    }

    NonnullRefPtr(AdoptTag, const T& object)
        : m_ptr(const_cast<T*>(&object))
    {
    }

    NonnullRefPtr(const NonnullRefPtr<T>& other)
        : m_ptr(other.m_ptr)
    {
        ref(m_ptr);
    }

    template<typename U>
    NonnullRefPtr(const NonnullRefPtr<U>& other)
        : m_ptr(const_cast<U*>(other.ptr()))
    {
        ref(m_ptr);
    }

    NonnullRefPtr<T>& operator=(NonnullRefPtr<T>& other)
    {
        NonnullRefPtr ptr(other);
        swap(ptr);
        return *this;
    }

    template<typename U>
    NonnullRefPtr<T>& operator=(NonnullRefPtr<U>& other)
    {
        NonnullRefPtr ptr(other);
        swap(ptr);
        return *this;
    }

    NonnullRefPtr(NonnullRefPtr<T>&& other)
        : m_ptr(other.leak_ref())
    {
    }

    template<typename U>
    NonnullRefPtr(NonnullRefPtr<U>&& other)
        : m_ptr(other.leak_ref())
    {
    }

    NonnullRefPtr<T>& operator=(NonnullRefPtr<T>&& other)
    {
        NonnullRefPtr ptr(other);
        swap(ptr);
        return *this;
    }

    template<typename U>
    NonnullRefPtr<T>& operator=(NonnullRefPtr<U>&& other)
    {
        NonnullRefPtr ptr(other);
        swap(ptr);
        return *this;
    }

    ~NonnullRefPtr()
    {
        unref(m_ptr);
#ifndef SANITIZE_PTRS
        if constexpr (sizeof(T*) == 8)
            m_ptr = (T*)0x1e1e1e1e1e1e1e1e;
        else
            m_ptr = (T*)0x1e1e1e1e;
#endif
    }

    T* ptr() { return m_ptr; }
    const T* ptr() const { return m_ptr; }

    T* operator->() { return m_ptr; }
    const T* operator->() const { return m_ptr; }

    T& operator*() { return *m_ptr; }
    const T& operator*() const { return *m_ptr; }

    T* leak_ref()
    {
        ASSERT(m_ptr);
        return exchange(m_ptr, nullptr);
    }

    operator bool() const = delete;
    bool operator!() const = delete;

    void swap(NonnullRefPtr<T>& other)
    {
        ::swap(m_ptr, other.m_ptr);
    }

    template<typename U>
    void swap(NonnullRefPtr<U>& other)
    {
        ::swap(m_ptr, other.m_ptr);
    }

private:
    T* m_ptr = nullptr;
};

template<typename T>
inline NonnullRefPtr<T> adopt(T& object)
{
    return NonnullRefPtr<T>(NonnullRefPtr<T>::Adopt, object);
}

template<typename T, typename U>
inline void swap(NonnullRefPtr<T>& a, NonnullRefPtr<U>& b)
{
    a.swap(b);
}

} // namespace ASL

using ASL::adopt;
using ASL::NonnullRefPtr;
using ASL::swap;
