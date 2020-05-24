#pragma once

#include <ASL/Forward.h>
#include <ASL/NumericLimits.h>
#include <ASL/Optional.h>
#include <ASL/StringImpl.h>
#include <ASL/Vector.h>

namespace ASL {

class LIB_API StringView {
public:
    using ConstIterator = const char*;
    static const size_t npos = NumericLimits<size_t>::max();

    StringView() {};
    StringView(const String&);
    StringView(const SharedString&);
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

    Optional<size_t> index_of(const StringView&, size_t offset = 0) const;
    Optional<size_t> last_index_of(const StringView&, size_t offset = npos) const;
    Optional<size_t> index_of(char, size_t offset = 0) const;
    Optional<size_t> last_index_of(char, size_t offset = npos) const;

    bool contains(char) const;

    StringView substring_view(size_t start, size_t length = npos) const;
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
