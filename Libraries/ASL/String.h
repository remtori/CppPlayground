#pragma once

#include "ByteBuffer.h"
#include "RefPtr.h"
#include "StringImpl.h"
#include "StringView.h"
#include "Traits.h"
#include "Vector.h"

namespace ASL {

class SharedString;

class String {
public:
    using ConstIterator = const char*;

    static String empty();
    static String number(unsigned);
    static String number(unsigned long);
    static String number(unsigned long long);
    static String number(int);
    static String number(long);
    static String number(long long);
    static String repeated(char, size_t count);

    String() {}
    ~String() {}

    String(const String& other)
        : m_impl(const_cast<String&>(other).m_impl)
    {
    }

    String(String&& other)
        : m_impl(move(other.m_impl))
    {
    }

    String(const char* cstring)
        : m_impl(StringImpl::from_chars(cstring))
    {
    }

    String(const char* cstring, size_t length)
        : m_impl(StringImpl::from_chars(cstring, length))
    {
    }

    String(const StringImpl& impl)
        : m_impl(const_cast<StringImpl&>(impl))
    {
    }

    String(const StringImpl* impl)
        : m_impl(const_cast<StringImpl*>(impl))
    {
    }

    String(RefPtr<StringImpl>&& impl)
        : m_impl(move(impl))
    {
    }

    String(NonnullRefPtr<StringImpl>&& impl)
        : m_impl(move(impl))
    {
    }

    String(const ByteBuffer& buffer)
        : m_impl(StringImpl::from_chars((const char*)buffer.data(), buffer.size()))
    {
    }

    String(const StringView&);
    String(const SharedString&);

    bool is_null() const { return !m_impl; }
    bool is_empty() const { return length() == 0; }
    size_t length() const { return m_impl ? m_impl->length() : 0; }
    const char* characters() const { return m_impl ? m_impl->characters() : nullptr; }
    const char& operator[](size_t i) const
    {
        return (*m_impl)[i];
    }

    ConstIterator begin() const { return characters(); }
    ConstIterator end() const { return begin() + length(); }

    bool starts_with(char) const;
    bool ends_with(char) const;
    bool starts_with(const StringView&) const;
    bool ends_with(const StringView&) const;

    String substring(size_t start, size_t length) const;
    StringView substring_view(size_t start, size_t length) const;
    StringView view() const;

    Vector<String> split_limit(char separator, size_t limit, bool keep_empty = false) const;
    Vector<String> split(char separator, bool keep_empty = false) const;
    Vector<StringView> split_view(char separator, bool keep_empty = false) const;

    StringImpl* impl() { return m_impl.ptr(); }
    const StringImpl* impl() const { return m_impl.ptr(); }

    String isolated_copy() const;

    String& operator=(String&& other)
    {
        if (this != &other)
            m_impl = move(other.m_impl);
        return *this;
    }

    String& operator=(const String& other)
    {
        if (this != &other)
            m_impl = const_cast<String&>(other).m_impl;
        return *this;
    }

    u32 hash() const
    {
        if (!m_impl)
            return 0;
        return m_impl->hash();
    }

    bool operator==(const String&) const;
    bool operator!=(const String& other) const { return !(*this == other); }
    bool operator==(const char* cstring) const;
    bool operator!=(const char* cstring) const { return !(*this == cstring); }

    bool operator<(const String&) const;
    bool operator<(const char*) const;
    bool operator>=(const String& other) const { return !(*this < other); }
    bool operator>=(const char* other) const { return !(*this < other); }

    bool operator>(const String&) const;
    bool operator>(const char*) const;
    bool operator<=(const String& other) const { return !(*this > other); }
    bool operator<=(const char* other) const { return !(*this > other); }

    bool operator==(const SharedString&) const;
    bool operator!=(const SharedString& other) const { return !(*this == other); }

private:
    RefPtr<StringImpl> m_impl;
};

template<>
struct Traits<String> : public GenericTraits<String> {
    static unsigned hash(const String& s) { return s.impl() ? s.impl()->hash() : 0; }
};

bool operator<(const char*, const String&);
bool operator>=(const char*, const String&);
bool operator>(const char*, const String&);
bool operator<=(const char*, const String&);

}; // namespace ASL

using ASL::String;
