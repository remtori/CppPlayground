#pragma once

#include "ByteBuffer.h"

namespace ASL {

class String;
class StringView;

class StringBuilder {
public:
    StringBuilder(size_t initial_capacity = 16);
    ~StringBuilder() {}

    String build() const;
    String to_string() const;

    StringView string_view() const;
    void clear();

    bool is_empty() const { return m_length == 0; }
    size_t length() const { return m_length; }

    void append(const StringView&);
    void append(char);

private:
    inline void will_append(size_t);

    ByteBuffer m_buffer;
    size_t m_length = 0;
};

} // namespace ASL
