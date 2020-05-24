#pragma once

#include <ASL/Forward.h>
#include <ASL/JsonArray.h>
#include <ASL/JsonObject.h>
#include <ASL/JsonValue.h>
#include <ASL/StringView.h>

namespace ASL {

class LIB_API JsonParser {
public:
    explicit JsonParser(const StringView& input)
        : m_input(input)
    {
    }

    ~JsonParser()
    {
    }

    JsonValue parse();

private:
    inline char peek() const;
    inline char consume();
    void consume_whitespace();
    void consume_specific(char);
    void consume_string(const char*);
    String consume_quoted_string();
    JsonArray parse_array();
    JsonObject parse_object();
    JsonValue parse_number();
    JsonValue parse_string();
    JsonValue parse_false();
    JsonValue parse_true();
    JsonValue parse_null();

    template<typename C>
    void consume_while(C);

    size_t m_index = 0;
    StringView m_input;
};

}; // namespace ASL

using ASL::JsonParser;
