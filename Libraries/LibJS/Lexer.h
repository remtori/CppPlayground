#pragma once

#include "Token.h"

namespace JS {

class Lexer {
public:
    explicit Lexer(StringView source);

    Token next_token();

private:
    void consume();
    bool is_eof() const;
    bool is_identifier_start() const;
    bool is_identifier_middle() const;
    bool is_line_comment_start() const;
    bool is_block_comment_start() const;
    bool is_block_comment_end() const;
    bool is_numeric_literal_start() const;
    bool match(char, char) const;
    bool match(char, char, char) const;
    bool match(char, char, char, char) const;

    StringView m_source;
    size_t m_position = 0;
    Token m_current_token;
    int m_current_char = 0;
    size_t m_line_number = 1;
    size_t m_line_column = 1;
};

} // namespace JS