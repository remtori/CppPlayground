#pragma once

#include "Optional.h"
#include "StringImpl.h"
#include "Vector.h"

namespace ASL {

class String;

class StringView {
public:
    using ConstIterator = const char*;

    StringView() {};
    StringView(const String& str);
    StringView(const char* cstring);
    StringView(const char* cstring, size_t length);

    bool is_null() const { return !m_characters; }
    bool is_empty() const { return m_length == 0; }
    size_t length() const { return m_length; }
    const char* characters_wont() const { return m_characters; }

    const char& operator[](size_t index) const
    {
        return m_characters[index];
    }

    ConstIterator begin() const { return characters_wont(); }
    ConstIterator end() const { return begin() + length(); }

    unsigned hash() const;

    bool starts_with(const StringView&) const;
    bool ends_with(const StringView&) const;
    bool starts_with(char) const;
    bool ends_with(char) const;

    bool contains(char) const;

    Optional<size_t> find_first_of(char) const;
    Optional<size_t> find_first_of(const StringView&) const;

    Optional<size_t> find_last_of(char) const;
    Optional<size_t> find_last_of(const StringView&) const;

    StringView substring_view(size_t start, size_t length) const;
    Vector<StringView> split_view(char, bool keep_empty = false) const;

    bool operator==(const char* cstring) const;
    bool operator!=(const char* cstring) const
    {
        return !(*this == cstring);
    }

    bool operator==(const String&) const;
    bool operator!=(const String& other)
    {
        return !(*this == other);
    }

    bool operator==(const StringView& other) const;
    bool operator!=(const StringView& other) const
    {
        return !(*this == other);
    }

    const StringImpl* impl() const { return m_impl; }

private:
    friend class String;
    const StringImpl* m_impl = nullptr;
    const char* m_characters = nullptr;
    size_t m_length = 0;
};

} // namespace ASL

using ASL::StringView;
