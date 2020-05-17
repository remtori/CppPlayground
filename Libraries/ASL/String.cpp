#include "String.h"

#include "SharedString.h"
#include "StringUtils.h"
#include "StringView.h"
#include <stdio.h>
#include <string.h>

namespace ASL {

inline char to_lower_case(char c)
{
    if ('A' <= c && c <= 'Z')
        return c + ('a' - 'A');
    return c;
}

String String::empty()
{
    return StringImpl::empty();
}

String String::number(unsigned value)
{
    char buffer[32];
    int size = sprintf(buffer, "%u", value);
    return String(buffer, size);
}

String String::number(unsigned long value)
{
    int size;
    char buffer[32];
    size = sprintf(buffer, "%lu", value);
    return String(buffer, size);
}

String String::number(unsigned long long value)
{
    int size;
    char buffer[32];
    size = sprintf(buffer, "%llu", value);
    return String(buffer, size);
}

String String::number(int value)
{
    char buffer[32];
    int size = sprintf(buffer, "%d", value);
    return String(buffer, size);
}

String String::number(long value)
{
    char buffer[32];
    int size = sprintf(buffer, "%ld", value);
    return String(buffer, size);
}

String String::number(long long value)
{
    char buffer[32];
    int size = sprintf(buffer, "%lld", value);
    return String(buffer, size);
}

String String::number(double value)
{
    char buffer[32];
    int size = sprintf(buffer, "%.4f", value);
    return String(buffer, size);
}

String String::repeated(char ch, size_t count)
{
    if (!count)
        return empty();

    char* buffer;
    auto impl = StringImpl::create_uninitialized(count, buffer);
    memset(buffer, ch, count);

    return *impl;
}

u32 String::to_uint(bool* ok) const
{
    return string_to_uint(*this, ok);
}

i32 String::to_int(bool* ok) const
{
    return string_to_int(*this, ok);
}

double String::to_double(bool* ok) const
{
    return string_to_double(*this, ok);
}

double String::to_double_as_exponent(bool* ok) const
{
    return string_e_to_double(*this, ok);
}

String::String(const StringView& view)
{
    if (view.m_impl)
        m_impl = *view.m_impl;
    else
        m_impl = StringImpl::from_chars(view.characters_wont(), view.length());
}

String::String(const SharedString& shared)
    : m_impl(shared.impl())
{
}

bool String::starts_with(const StringView& str, bool case_sensitive) const
{
    if (str.is_empty())
        return true;

    if (is_empty())
        return false;

    if (str.length() > length())
        return false;

    if (characters() == str.characters_wont())
        return true;

    if (case_sensitive)
        return !memcmp(characters(), str.characters_wont(), str.length());

    for (size_t i = 0; i < str.length(); ++i)
        if (to_lower_case(characters()[i]) != to_lower_case(str[i]))
            return false;

    return true;
}

bool String::ends_with(const StringView& str, bool case_sensitive) const
{
    if (str.is_empty())
        return true;

    if (is_empty())
        return false;

    if (str.length() > length())
        return false;

    if (case_sensitive)
        return !memcmp(characters() + length() - str.length(), str.characters_wont(), str.length());

    for (size_t i = 0; i < str.length(); ++i)
        if (to_lower_case(characters()[i + length() - str.length()]) != to_lower_case(str[i]))
            return false;

    return true;
}

bool String::starts_with(char c, bool case_sensitive) const
{
    if (is_empty())
        return false;

    if (case_sensitive)
        return characters()[0] == c;

    return to_lower_case(characters()[0]) == to_lower_case(c);
}

bool String::ends_with(char c, bool case_sensitive) const
{
    if (is_empty())
        return false;

    if (case_sensitive)
        return characters()[length() - 1] == c;

    return to_lower_case(characters()[length() - 1]) == to_lower_case(c);
}

bool String::equals(const StringView& str, bool case_sensitive)
{
    if (str.is_empty())
        return is_empty();

    if (is_empty())
        return false;

    if (str.length() != length())
        return false;

    if (case_sensitive)
        return !memcmp(characters(), str.characters_wont(), str.length());

    for (size_t i = 0; i < str.length(); ++i)
        if (to_lower_case(characters()[i]) != to_lower_case(str[i]))
            return false;

    return true;
}

Optional<size_t> String::index_of(const StringView& str, size_t offset) const
{
    if (length() < str.length())
        return {};

    for (size_t i = offset; i < length() - str.length(); ++i)
        if (str == substring_view(i, str.length()))
            return i;

    return {};
}

Optional<size_t> String::last_index_of(const StringView& str, size_t offset) const
{
    if (length() < str.length())
        return {};

    if (offset == npos)
        offset = length();

    size_t i = min(offset, length()) - str.length();
    while (i--) {
        if (str == substring_view(i, str.length()))
            return i;
    }

    return {};
}

Optional<size_t> String::index_of(char c, size_t offset) const
{
    for (size_t i = offset; i < length(); ++i)
        if (characters()[i] == c)
            return i;

    return {};
}

Optional<size_t> String::last_index_of(char c, size_t offset) const
{
    if (offset == npos)
        offset = length();

    size_t i = min(offset, length()) - 1;
    while (i--) {
        if (characters()[i] == c)
            return i;
    }

    return {};
}

String String::substring(size_t start, size_t length) const
{
    if (!length)
        return {};

    ASSERT(m_impl);
    if (length == npos)
        length = m_impl->length() - start;

    ASSERT(start + length <= m_impl->length());

    return { characters() + start, length };
}

StringView String::substring_view(size_t start, size_t length) const
{
    ASSERT(m_impl);
    if (length == npos)
        length = m_impl->length() - start;

    ASSERT(start + length <= m_impl->length());

    return { characters() + start, length };
}

StringView String::view() const
{
    return { characters(), length() };
}

Vector<String> String::split_limit(char separator, size_t limit, bool keep_empty) const
{
    if (is_empty())
        return {};

    Vector<String> v;

    size_t start = 0;
    for (size_t i = 0; i < length(); ++i) {
        if (characters()[i] == separator) {
            size_t len = i - start;
            if (len != 0 || keep_empty) {
                v.append(substring(start, len));
                if (v.size() == limit)
                    break;
            }

            start = i + 1;
        }
    }

    size_t len = length() - start;
    if (len != 0 || keep_empty)
        v.append(substring(start, len));

    return v;
}

Vector<String> String::split(char separator, bool keep_empty) const
{
    return split_limit(separator, 0, keep_empty);
}

Vector<StringView> String::split_view(char separator, bool keep_empty) const
{
    if (is_empty())
        return {};

    Vector<StringView> v;

    size_t start = 0;
    for (size_t i = 0; i < length(); ++i) {
        if (characters()[i] == separator) {
            size_t len = i - start;
            if (len != 0 || keep_empty)
                v.append(substring_view(start, len));

            start = i + 1;
        }
    }

    size_t len = length() - start;
    if (len != 0 || keep_empty)
        v.append(substring_view(start, len));

    return v;
}

String String::isolated_copy() const
{
    if (!m_impl)
        return {};

    if (!m_impl->length())
        return empty();

    char* buffer;
    auto impl = StringImpl::create_uninitialized(length(), buffer);
    memcpy(buffer, m_impl->characters(), m_impl->length());

    return String(move(*impl));
}

bool String::operator==(const String& other) const
{
    if (!m_impl)
        return !other.m_impl;

    if (!other.m_impl)
        return false;

    if (length() != other.length())
        return false;

    return !memcmp(characters(), other.characters(), length());
}

bool String::operator==(const char* cstring) const
{
    if (!m_impl)
        return !cstring;

    if (!cstring)
        return false;

    return !strcmp(characters(), cstring);
}

bool String::operator==(const SharedString& other) const
{
    return *this == String(other.impl());
}

bool String::operator<(const String& other) const
{
    if (!m_impl)
        return other.m_impl;

    if (!other.m_impl)
        return false;

    return strcmp(characters(), other.characters()) < 0;
}

bool String::operator<(const char* cstring) const
{
    if (!m_impl)
        return cstring;

    if (!cstring)
        return false;

    return strcmp(characters(), cstring) < 0;
}

bool String::operator>(const String& other) const
{
    if (!m_impl)
        return other.m_impl;

    if (!other.m_impl)
        return false;

    return strcmp(characters(), other.characters()) > 0;
}

bool String::operator>(const char* cstring) const
{
    if (!m_impl)
        return cstring;

    if (!cstring)
        return false;

    return strcmp(characters(), cstring) > 0;
}

bool operator<(const char* characters, const String& string)
{
    if (!characters)
        return !string.is_null();

    if (string.is_null())
        return false;

    return strcmp(characters, string.characters()) < 0;
}

bool operator>=(const char* characters, const String& string)
{
    return !(characters < string);
}

bool operator>(const char* characters, const String& string)
{
    if (!characters)
        return !string.is_null();

    if (string.is_null())
        return false;

    return strcmp(characters, string.characters()) > 0;
}

bool operator<=(const char* characters, const String& string)
{
    return !(characters > string);
}

} // namespace ASL
