#pragma once

#include "ByteBuffer.h"
#include "Demangle.h"
#include "Forward.h"
#include "HashMap.h"
#include "NumericLimits.h"
#include "Optional.h"
#include "RefPtr.h"
#include "StringImpl.h"
#include "StringView.h"
#include "Traits.h"
#include "Vector.h"
#include <stdio.h>

namespace ASL {

class LIB_API String {
public:
    using ConstIterator = const char*;
    static const size_t npos = NumericLimits<size_t>::max();

    static String empty();
    static String number(unsigned);
    static String number(unsigned long);
    static String number(unsigned long long);
    static String number(int);
    static String number(long);
    static String number(long long);
    static String number(double);
    static String repeated(char, size_t count);
    template<class... Args>
    static String format(const StringView& fmt, const Args&... input_args);

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

    String(char c)
        : m_impl(StringImpl::from_chars(&c, 1))
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

    u32 to_uint(bool* ok = nullptr) const;
    i32 to_int(bool* ok = nullptr) const;
    double to_double(bool* ok = nullptr) const;
    double to_double_as_exponent(bool* ok = nullptr) const;

    ConstIterator begin() const { return characters(); }
    ConstIterator end() const { return begin() + length(); }

    bool starts_with(char, bool case_sensitive = true) const;
    bool ends_with(char, bool case_sensitive = true) const;
    bool starts_with(const StringView&, bool case_sensitive = true) const;
    bool ends_with(const StringView&, bool case_sensitive = true) const;
    bool equals(const StringView&, bool case_sensitive = true);

    Optional<size_t> index_of(const StringView&, size_t offset = 0) const;
    Optional<size_t> last_index_of(const StringView&, size_t offset = npos) const;
    Optional<size_t> index_of(char, size_t offset = 0) const;
    Optional<size_t> last_index_of(char, size_t offset = npos) const;

    String substring(size_t start, size_t length = npos) const;
    StringView substring_view(size_t start, size_t length = npos) const;
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

LIB_API bool operator<(const char*, const String&);
LIB_API bool operator>=(const char*, const String&);
LIB_API bool operator>(const char*, const String&);
LIB_API bool operator<=(const char*, const String&);

////////////////////////// String format implementation //////////////////////////////

struct FieldFormat {
    bool is_raw = false;
    int padding = 0;
    int precision = 4;
    size_t field_index = -1;
    StringView string;

    FieldFormat() {}
    ~FieldFormat() {}

    FieldFormat(bool raw, const StringView& str)
        : is_raw(raw)
        , string(str)
    {
    }
};

LIB_API String extract_field_format(const StringView&, Vector<FieldFormat>&);
LIB_API String build_result(Vector<FieldFormat> field_formats, HashMap<size_t, String> field_values);

template<class T>
constexpr auto call_to_string_if_present(const T& obj, String& result) -> decltype(obj.to_string(), TrueType {})
{
    result = obj.to_string();
    return {};
}

constexpr auto call_to_string_if_present(...) -> FalseType { return {}; }

template<typename T>
String try_to_string(const T& any)
{
    if constexpr (CanConstructString<T>::value)
        return any;

    String result;
    if constexpr (IsPointer<T>::value)
        call_to_string_if_present(*any, result);
    else
        call_to_string_if_present(any, result);

    if (!result.is_empty())
        return result;

    static char buf[96];
    String class_name = demangle(typeid(T).name());
    size_t size = snprintf(buf, 96, "<error>Can not stringify %s </error>", class_name.characters());
    return { buf, size };
}

template<typename T>
static constexpr String format_argument(const T& value, FieldFormat format)
{
    if constexpr (IsInteger<T>::value)
        return String::number(value);

    if constexpr (IsSame<T, float>::value) {
        char buf[32];
        int size = sprintf(buf, "%.*f", format.precision, value);
        return String(buf, size);
    }

    if constexpr (IsSame<T, double>::value) {
        char buf[32];
        int size = sprintf(buf, "%.*lf", format.precision, value);
        return String(buf, size);
    }

    return try_to_string(value);
}

template<class... Args>
String String::format(const StringView& fmt, const Args&... input_args)
{
    // TODO: Maybe we can also format value to hex or binary format
    Vector<FieldFormat> field_formats;

    String error = extract_field_format(fmt, field_formats);
    if (!error.is_empty())
        return error;

    HashMap<size_t, String> field_values;

    size_t input_arg_count = 0;
    auto index_finder = [&input_arg_count](const FieldFormat& f) {
        return f.field_index == input_arg_count;
    };

    auto resolve = [&](auto&& arg) {
        auto it = field_formats.find(index_finder);

        while (it != field_formats.end()) {
            field_values.set(it.index(), format_argument(arg, *it));
            it = field_formats.find(index_finder, it.index() + 1);
        }

        ++input_arg_count;
    };

    (resolve(input_args), ...);

    return build_result(field_formats, field_values);
}

}; // namespace ASL

using ASL::String;
