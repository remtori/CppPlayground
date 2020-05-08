#include "String.h"

#include <stdio.h>
#include <string.h>

namespace ASL {

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

String String::repeated(char ch, size_t count)
{
    if (!count)
        return empty();

    char* buffer;
    auto impl = StringImpl::create_uninitialized(count, buffer);
    memset(buffer, ch, count);

    return *impl;
}

bool String::starts_with(const StringView& str) const
{
    if (str.is_empty())
        return true;

    if (is_empty())
        return false;

    if (str.length() > length())
        return false;

    if (characters() == str.characters_wont())
        return true;

    return !memcmp(characters(), str.characters_wont(), str.length());
}

bool String::ends_with(const StringView& str) const
{
    if (str.is_empty())
        return true;

    if (is_empty())
        return false;

    if (str.length() > length())
        return false;

    return !memcmp(characters() + length() - str.length(), str.characters_wont(), str.length());
}

bool String::starts_with(char ch) const
{
    if (is_empty())
        return false;
    return characters()[0] == ch;
}

bool String::ends_with(char ch) const
{
    if (is_empty())
        return false;
    return characters()[length() - 1] == ch;
}

String String::substring(size_t start, size_t length) const
{
    if (!length)
        return {};

    ASSERT(m_impl);
    ASSERT(start + length <= m_impl->length());

    return { characters() + start, length };
}

StringView String::substring_view(size_t start, size_t length) const
{
    ASSERT(m_impl);
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
    for (size_t i = 0; i < length(); i++) {
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
