#pragma once

#include "Assertions.h"

namespace ASL {

class RefCountedBase {
public:
    void ref() const
    {
        ASSERT(m_ref_count);
        ++m_ref_count;
    }

    int ref_count() const
    {
        return m_ref_count;
    }

protected:
    RefCountedBase() {}
    ~RefCountedBase()
    {
        ASSERT(!m_ref_count);
    }

    void deref_base() const
    {
        ASSERT(m_ref_count);
        --m_ref_count;
    }

    mutable int m_ref_count = 1;
};

template<typename T>
class RefCounted : public RefCountedBase {
public:
    void unref() const
    {
        deref_base();
        if (m_ref_count == 0)
            delete static_cast<const T*>(this);
    }
};

} // namespace ASL

using ASL::RefCounted;
