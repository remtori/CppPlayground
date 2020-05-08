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

bool StringView::contains(char c) const
{
    for (char ch : *this) {
        if (ch == c)
            return true;
    }

    return false;
}

Optional<size_t> StringView::find_first_of(char c) const
{
    for (size_t pos = 0; pos < m_length; ++pos) {
        if (m_characters[pos] == c)
            return pos;
    }

    return {};
}

Optional<size_t> StringView::find_first_of(const StringView& view) const
{
    for (size_t pos = 0; pos < m_length; ++pos) {
        char c = m_characters[pos];
        for (char view_char : view) {
            if (c == view_char)
                return pos;
        }
    }

    return {};
}

Optional<size_t> StringView::find_last_of(char c) const
{
    for (size_t pos = m_length; pos > 0; --pos) {
        if (m_characters[pos] == c)
            return pos;
    }

    return {};
}

Optional<size_t> StringView::find_last_of(const StringView& view) const
{
    for (size_t pos = m_length - 1; pos > 0; --pos) {
        char c = m_characters[pos];
        for (char view_char : view) {
            if (c == view_char)
                return pos;
        }
    }

    return {};
}

StringView StringView::substring_view(size_t start, size_t length) const
{
    ASSERT(start + length <= m_length);
    return { m_characters + start, length };
}

Vector<StringView> StringView::split_view(char separator, bool keep_empty) const
{
    if (is_empty())
        return {};

    Vector<StringView> v;

    size_t start = 0;
    for (size_t i = 0; i < m_length; i++) {
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
