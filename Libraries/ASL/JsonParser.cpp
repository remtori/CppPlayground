#include "JsonParser.h"

#include "Vector.h"

namespace ASL {

static inline bool is_whitespace(char c)
{
    return c == ' ' || c == '\n' || c == '\r' || c == '\t';
}

static inline bool is_number(char c)
{
    return ('0' <= c && c <= '9') || c == '-' || c == '+' || c == '.' || c == 'e' || c == 'E';
}

JsonValue JsonParser::parse()
{
    consume_whitespace();
    switch (peek()) {
    case '{':
        return parse_object();
    case '[':
        return parse_array();
    case '"':
        return parse_string();
    case '-':
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        return parse_number();
    case 'f':
        return parse_false();
    case 't':
        return parse_true();
    case 'n':
        return parse_null();
    }

    return JsonValue();
}

inline char JsonParser::peek() const
{
    if (m_index < m_input.length())
        return m_input[m_index];

    return '\0';
}

inline char JsonParser::consume()
{
    if (m_index < m_input.length())
        return m_input[m_index++];

    return '\0';
}

template<typename C>
void JsonParser::consume_while(C condition)
{
    while (condition(peek()))
        consume();
}

void JsonParser::consume_whitespace()
{
    consume_while([](char c) { return is_whitespace(c); });
}

void JsonParser::consume_specific(char expected)
{
    char consumed = consume();
    ASSERT(consumed == expected);
}

void JsonParser::consume_string(const char* cstring)
{
    for (size_t i = 0, length = strlen(cstring); i < length; ++i)
        consume_specific(cstring[i]);
}

String JsonParser::consume_quoted_string()
{
    consume_specific('"');
    Vector<char> buffer;

    for (;;) {
        size_t peek_index = m_index;
        char c = '\0';
        for (;;) {
            if (peek_index == m_input.length())
                break;

            c = m_input[peek_index];
            if (c == '"' || c == '\\')
                break;

            ++peek_index;
        }

        if (peek_index != m_index) {
            buffer.append(m_input.characters_wont() + m_index, peek_index - m_index);
            m_index = peek_index;
        }

        if (peek_index == m_input.length())
            break;

        if (c == '"') {
            consume();
            break;
        }

        consume_specific('\\');
        char escaped_char = consume();
        switch (escaped_char) {
        case 'b':
            buffer.append('\b');
            break;
        case 'f':
            buffer.append('\f');
            break;
        case 'n':
            buffer.append('\n');
            break;
        case 'r':
            buffer.append('\r');
            break;
        case 't':
            buffer.append('\t');
            break;
        case 'u':
            // Unicode char
            consume();
            consume();
            consume();
            consume();
            // Not supported yet
            buffer.append('?');
            break;
        default:
            buffer.append(escaped_char);
            break;
        }
    }

    if (buffer.is_empty())
        return String::empty();

    return String(buffer.data(), buffer.size());
}

JsonArray JsonParser::parse_array()
{
    consume_specific('[');

    JsonArray array;
    for (;;) {
        consume_whitespace();
        if (peek() == ']')
            break;

        array.append(parse());

        consume_whitespace();
        if (peek() == ']')
            break;

        consume_specific(',');
    }
    consume_specific(']');

    return array;
}

JsonObject JsonParser::parse_object()
{
    consume_specific('{');
    JsonObject obj;

    for (;;) {
        consume_whitespace();
        if (peek() == '}')
            break;

        String key = consume_quoted_string();

        consume_whitespace();
        consume_specific(':');

        consume_whitespace();
        JsonValue value = parse();
        obj.set(key, move(value));

        consume_whitespace();
        if (peek() == '}')
            break;

        consume_specific(',');
    }
    consume_specific('}');

    return obj;
}

JsonValue JsonParser::parse_number()
{
    size_t index = m_index;
    consume_while([](char c) { return is_number(c); });

    String str(m_input.substring_view(index, m_index - index));

    bool ok;
    u32 uint_v = str.to_uint(&ok);
    if (ok)
        return uint_v;

    i32 int_v = str.to_int(&ok);
    if (ok)
        return int_v;

    double double_v = str.to_double(&ok);
    if (ok)
        return double_v;

    return JsonValue(JsonValue::Type::Null);
}

JsonValue JsonParser::parse_string()
{
    return consume_quoted_string();
}

JsonValue JsonParser::parse_false()
{
    consume_string("false");
    return JsonValue(false);
}

JsonValue JsonParser::parse_true()
{
    consume_string("true");
    return JsonValue(true);
}

JsonValue JsonParser::parse_null()
{
    consume_string("null");
    return JsonValue(JsonValue::Type::Null);
}

} // namespace ASL