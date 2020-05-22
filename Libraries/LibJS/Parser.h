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

    NonnullRefPtr<Statement> parse_statement();

    NonnullRefPtr<Expression> parse_expression(int min_precedence, Associativity associate = Associativity::Right);
    NonnullRefPtr<Expression> parse_primary_expression();
    NonnullRefPtr<Expression> parse_secondary_expression(NonnullRefPtr<Expression>, int min_precedence, Associativity associativity = Associativity::Right);
    NonnullRefPtr<AssignmentExpression> parse_assignment_expression(AssignmentOp op, NonnullRefPtr<Expression> lhs, NonnullRefPtr<Expression> rhs);
    NonnullRefPtr<UnaryExpression> parse_unary_expression();
    NonnullRefPtr<VariableDeclaration> parse_variable_declaration();

private:
    bool done() const;
    Token consume();
    Token consume(TokenType);
    void consume_semicolon();
    void expected(const char*);
    void syntax_error(const String& message, size_t line = 0, size_t column = 0);
    int operator_precedence(TokenType type) const;

    bool match_statement() const;
    bool match_expression() const;
    bool match_unary_expression() const;
    bool match_secondary_expression() const;

    Associativity operator_associativity(TokenType type) const;

    Lexer m_lexer;
    Token m_current_token;
};

} // namespace JS
