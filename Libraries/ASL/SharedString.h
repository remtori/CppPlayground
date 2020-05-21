#pragma once

#include "String.h"
#include "StringView.h"

namespace ASL {

class LIB_API SharedString {
public:
    SharedString() {}
    SharedString(const SharedString& other)
        : m_impl(other.impl())
    {
    }
    SharedString(SharedString&& other)
        : m_impl(move(other.m_impl))
    {
    }

    SharedString(const StringImpl*);
    SharedString(const String&);
    SharedString(const StringView&);
    SharedString(const char*);

    SharedString& operator=(const SharedString& other)
    {
        m_impl = other.m_impl;
        return *this;
    }

    SharedString& operator=(SharedString&& other)
    {
        m_impl = move(other.m_impl);
        return *this;
    }

    bool is_empty() const { return !m_impl || !m_impl->length(); }
    bool is_null() const { return !m_impl; }
    const char* characters() const { return m_impl ? m_impl->characters() : nullptr; }
    size_t length() const { return m_impl ? m_impl->length() : 0; }

    const StringImpl* impl() const { return m_impl.ptr(); }

    unsigned hash() const { return m_impl ? m_impl->existing_hash() : 0; }
    StringView view() const;

    bool operator==(const SharedString& other) const { return m_impl == other.m_impl; }
    bool operator!=(const SharedString& other) const { return m_impl != other.m_impl; }

    bool operator==(const String&) const;
    bool operator!=(const String& string) const { return !(*this == string); }

    bool operator==(const StringView&) const;
    bool operator!=(const StringView& string) const { return !(*this == string); }

    bool operator==(const char*) const;
    bool operator!=(const char* string) const { return !(*this == string); }

    static void will_destroy_impl(StringImpl*);

private:
    RefPtr<StringImpl> m_impl;
};

template<>
struct Traits<SharedString> : public GenericTraits<SharedString> {
    static unsigned hash(const SharedString& s) { return s.hash(); }
};

} // namespace ASL

using ASL::SharedString;
