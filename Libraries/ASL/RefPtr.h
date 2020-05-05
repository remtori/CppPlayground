#pragma once

#include "NonnullOwnPtr.h"
#include "NonnullRefPtr.h"
#include "OwnPtr.h"
#include "StdLibExtras.h"

namespace ASL {

template<typename T>
class RefPtr {
public:
    enum AdoptTag { Adopt };

    RefPtr() {};

    RefPtr(AdoptTag, T& object)
        : m_ptr(&object)
    {
    }

    /////////// From raw //////////////

    RefPtr(const T* ptr)
        : m_ptr(const_cast<T*>(ptr))
    {
        ref(m_ptr);
    }

    RefPtr(const T& object)
        : m_ptr(const_cast<T*>(&object))
    {
        ref(m_ptr);
    }

    RefPtr& operator=(const T* ptr)
    {
        RefPtr tmp(ptr);
        swap(tmp);
        return *this;
    }

    RefPtr& operator=(const T& object)
    {
        RefPtr tmp(object);
        swap(tmp);
        return *this;
    }

    /////////// Copy from RefPtr //////////////

    RefPtr(RefPtr<T>& other)
        : m_ptr(other.ptr())
    {
        ref(m_ptr);
    }

    template<typename U>
    RefPtr(RefPtr<U>& other)
        : m_ptr(other.ptr())
    {
        ref(m_ptr);
    }

    RefPtr& operator=(RefPtr& other)
    {
        RefPtr tmp(other);
        swap(tmp);
        return *this;
    }

    template<typename U>
    RefPtr& operator=(RefPtr<U>& other)
    {
        RefPtr tmp(other);
        swap(tmp);
        return *this;
    }

    /////////// Move from RefPtr //////////////

    RefPtr(RefPtr<T>&& other)
        : m_ptr(other.leak_ref())
    {
    }

    template<typename U>
    RefPtr(RefPtr<U>&& other)
        : m_ptr(other.leak_ref())
    {
    }

    RefPtr& operator=(RefPtr&& other)
    {
        RefPtr tmp(move(other));
        swap(tmp);
        return *this;
    }

    template<typename U>
    RefPtr& operator=(RefPtr<U>&& other)
    {
        RefPtr tmp(move(other));
        swap(tmp);
        return *this;
    }

    /////////// Copy from NonnullRefPtr //////////////

    RefPtr(const NonnullRefPtr<T>& other)
        : m_ptr(const_cast<T*>(other.ptr()))
    {
        ASSERT(m_ptr);
        ref(m_ptr);
    }

    template<typename U>
    RefPtr(const NonnullRefPtr<U>& other)
        : m_ptr(const_cast<U*>(other.ptr()))
    {
        ASSERT(m_ptr);
        ref(m_ptr);
    }

    RefPtr& operator=(const NonnullRefPtr<T>& other)
    {
        RefPtr tmp = other;
        swap(tmp);
        ASSERT(m_ptr);
        return *this;
    }

    template<typename U>
    RefPtr& operator=(const NonnullRefPtr<U>& other)
    {
        RefPtr tmp = other;
        swap(tmp);
        ASSERT(m_ptr);
        return *this;
    }

    /////////// Move from NonnullRefPtr //////////////

    RefPtr(NonnullRefPtr<T>&& other)
        : m_ptr(other.leak_ref())
    {
        ASSERT(m_ptr);
    }

    template<typename U>
    RefPtr(NonnullRefPtr<U>&& other)
        : m_ptr(other.leak_ref())
    {
        ASSERT(m_ptr);
    }

    RefPtr& operator=(NonnullRefPtr<T>&& other)
    {
        RefPtr tmp = move(other);
        swap(tmp);
        ASSERT(m_ptr);
        return *this;
    }

    template<typename U>
    RefPtr& operator=(NonnullRefPtr<U>&& other)
    {
        RefPtr tmp = move(other);
        swap(tmp);
        ASSERT(m_ptr);
        return *this;
    }

    RefPtr& operator=(std::nullptr_t)
    {
        clear();
        return *this;
    }

    void clear()
    {
        unref(m_ptr);
        m_ptr = nullptr;
    }

    ~RefPtr()
    {
        clear();
#ifndef SANITIZE_PTRS
        if constexpr (sizeof(T*) == 8)
            m_ptr = (T*)0x1d1d1d1d1d1d1d1d;
        else
            m_ptr = (T*)0x1d1d1d1d;
#endif
    }

    T* ptr() { return m_ptr; }
    const T* ptr() const { return m_ptr; }

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

    T* leak_ref()
    {
        return exchange(m_ptr, nullptr);
    }

    NonnullRefPtr<T> release_nonnull()
    {
        ASSERT(m_ptr);
        return NonnullRefPtr<T>(NonnullRefPtr<T>::Adopt, *leak_ref());
    }

    operator bool() const { return m_ptr != nullptr; };
    bool operator!() const { return m_ptr == nullptr; };

    bool operator==(std::nullptr_t) const { return !m_ptr; }
    bool operator!=(std::nullptr_t) const { return m_ptr; }

    bool operator==(const RefPtr& other) const { return m_ptr == other.m_ptr; }
    bool operator!=(const RefPtr& other) const { return m_ptr != other.m_ptr; }

    bool operator==(RefPtr& other) { return m_ptr == other.m_ptr; }
    bool operator!=(RefPtr& other) { return m_ptr != other.m_ptr; }

    bool operator==(const T* other) const { return m_ptr == other; }
    bool operator!=(const T* other) const { return m_ptr != other; }

    bool operator==(T* other) { return m_ptr == other; }
    bool operator!=(T* other) { return m_ptr != other; }

    bool is_null() const { return !m_ptr; }

    void swap(RefPtr<T>& other)
    {
        ::swap(m_ptr, other.m_ptr);
    }

    template<typename U>
    void swap(RefPtr<U>& other)
    {
        ::swap(m_ptr, other.m_ptr);
    }

private:
    T* m_ptr = nullptr;
};

template<typename T, typename U>
inline NonnullRefPtr<T> static_ptr_cast(const NonnullRefPtr<U>& ptr)
{
    return NonnullRefPtr<T>(static_cast<const T&>(*ptr));
}

template<typename T, typename U>
inline RefPtr<T> static_ptr_cast(const RefPtr<U>& ptr)
{
    return RefPtr<T>(static_cast<const T*>(ptr.ptr()));
}

} // namespace ASL

using ASL::RefPtr;
using ASL::static_ptr_cast;
