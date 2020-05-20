#pragma once

#include "AST.h"
#include "Lexer.h"
#include <ASL/String.h>

namespace JS {

enum class Associativity {
    Left,
    Right,
};

class Parser {
public:
    explicit Parser(StringView&);

    NonnullRefPtr<Program> parse_program();
    NonnullRefPtr<Expression> parse_expression(int min_precedence, Associativity associate = Associativity::Right);
    NonnullRefPtr<Expression> parse_primary_expression();
    NonnullRefPtr<Expression> parse_secondary_expression(NonnullRefPtr<Expression>, int min_precedence, Associativity associate = Associativity::Right);
    NonnullRefPtr<UnaryExpression> parse_unary_expression();

private:
    Token consume();
    Token consume(TokenType);
    void expected(const char*);
    void syntax_error(const String& message, size_t line = 0, size_t column = 0);
    bool done() const;
    bool match_expression() const;
    bool match_unary_expression() const;
    bool match_secondary_expression() const;
    int operator_precedence(TokenType type) const;

    Associativity operator_associativity(TokenType type) const;

    Lexer m_lexer;
    Token m_current_token;
};

} // namespace JS
