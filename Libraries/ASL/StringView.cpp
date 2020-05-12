#include "StringView.h"

#include "SharedString.h"
#include "String.h"
#include <string.h>

namespace ASL {

StringView::StringView(const String& str)
    : m_impl(str.impl())
    , m_characters(str.characters())
    , m_length(str.length())
{
}

StringView::StringView(const SharedString& shared)
    : m_impl(shared.impl())
    , m_characters(shared.characters())
    , m_length(shared.length())
{
}

StringView::StringView(const char* cstring)
    : StringView(cstring, cstring ? strlen(cstring) : 0)
{
}

StringView::StringView(const char* cstring, size_t length)
    : m_characters(cstring)
    , m_length(length)
{
}

unsigned StringView::hash() const
{
    if (is_empty())
        return 0;
    if (m_impl)
        return m_impl->hash();

    return string_hash(m_characters, m_length);
}

bool StringView::starts_with(const StringView& str) const
{
    if (str.is_empty())
        return true;

    if (is_empty())
        return false;

    if (str.length() > length())
        return false;

    if (characters_wont() == str.characters_wont())
        return true;

    return !memcmp(characters_wont(), str.characters_wont(), str.length());
}

bool StringView::ends_with(const StringView& str) const
{
    if (str.is_empty())
        return true;

    if (is_empty())
        return false;

    if (str.length() > length())
        return false;

    return !memcmp(characters_wont() + length() - str.length(), str.characters_wont(), str.length());
}

bool StringView::starts_with(char c) const
{
    if (is_empty())
        return false;

    return c == m_characters[0];
}

bool StringView::ends_with(char c) const
{
    if (is_empty())
        return false;

    return c == m_characters[m_length - 1];
}

Optional<size_t> StringView::index_of(const StringView& str, size_t offset) const
{
    if (length() < str.length())
        return {};

    for (size_t i = offset; i < length() - str.length(); ++i)
        if (str == substring_view(i, str.length()))
            return i;

    return {};
}

Optional<size_t> StringView::last_index_of(const StringView& str, size_t offset) const
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

Optional<size_t> StringView::index_of(char c, size_t offset) const
{
    for (size_t i = offset; i < length(); ++i)
        if (characters_wont()[i] == c)
            return i;

    return {};
}

Optional<size_t> StringView::last_index_of(char c, size_t offset) const
{
    if (offset == npos)
        offset = length();

    size_t i = min(offset, length()) - 1;
    while (i--) {
        if (characters_wont()[i] == c)
            return i;
    }

    return {};
}

bool StringView::contains(char c) const
{
    for (char ch : *this) {
        if (ch == c)
            return true;
    }

    return false;
}

StringView StringView::substring_view(size_t start, size_t length) const
{
    if (length == npos)
        length = m_length - start;

    ASSERT(start + length <= m_length);
    return { m_characters + start, length };
}

Vector<StringView> StringView::split_view(char separator, bool keep_empty) const
{
    if (is_empty())
        return {};

    Vector<StringView> v;

    size_t start = 0;
    for (size_t i = 0; i < m_length; ++i) {
        if (m_characters[i] == separator) {
            size_t len = i - start;
            if (len != 0 || keep_empty)
                v.append(substring_view(start, len));

            start = i + 1;
        }
    }

    size_t len = m_length - start;
    if (len != 0 || keep_empty)
        v.append(substring_view(start, len));

    return v;
}

bool StringView::operator==(const char* cstring) const
{
    if (is_null())
        return !cstring;

    if (!cstring)
        return false;

    size_t other_length = strlen(cstring);
    if (m_length != other_length)
        return false;

    return !memcmp(m_characters, cstring, m_length);
}

bool StringView::operator==(const String& other) const
{
    if (is_null())
        return other.is_null();

    if (other.is_null())
        return false;

    if (length() != other.length())
        return false;

    return !memcmp(m_characters, other.characters(), m_length);
}

bool StringView::operator==(const StringView& other) const
{
    if (is_null())
        return other.is_null();

    if (other.is_null())
        return false;

    if (length() != other.length())
        return false;

    return !memcmp(m_characters, other.m_characters, m_length);
}

} // namespace ASL
