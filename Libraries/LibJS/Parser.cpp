#include "Parser.h"

#include <ASL/HashMap.h>
#include <ASL/LogStream.h>

namespace JS {

static HashMap<TokenType, int> g_operator_precedence;

Parser::Parser(StringView& source)
    : m_lexer(source)
    , m_current_token(m_lexer.next_token())
{
    if (g_operator_precedence.is_empty()) {
        // https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Operator_Precedence

        g_operator_precedence.set(TokenType::Period, 20);
        g_operator_precedence.set(TokenType::BracketOpen, 20);
        g_operator_precedence.set(TokenType::ParenOpen, 20);
        g_operator_precedence.set(TokenType::QuestionMarkPeriod, 20);

        g_operator_precedence.set(TokenType::New, 19);

        g_operator_precedence.set(TokenType::PlusPlus, 18);
        g_operator_precedence.set(TokenType::MinusMinus, 18);

        g_operator_precedence.set(TokenType::ExclamationMark, 17);
        g_operator_precedence.set(TokenType::Tilde, 17);
        g_operator_precedence.set(TokenType::Typeof, 17);
        g_operator_precedence.set(TokenType::Void, 17);
        g_operator_precedence.set(TokenType::Delete, 17);
        g_operator_precedence.set(TokenType::Await, 17);

        g_operator_precedence.set(TokenType::DoubleAsterisk, 16);

        g_operator_precedence.set(TokenType::Asterisk, 15);
        g_operator_precedence.set(TokenType::Slash, 15);
        g_operator_precedence.set(TokenType::Percent, 15);

        g_operator_precedence.set(TokenType::Plus, 14);
        g_operator_precedence.set(TokenType::Minus, 14);

        g_operator_precedence.set(TokenType::ShiftLeft, 13);
        g_operator_precedence.set(TokenType::ShiftRight, 13);
        g_operator_precedence.set(TokenType::UnsignedShiftRight, 13);

        g_operator_precedence.set(TokenType::LessThan, 12);
        g_operator_precedence.set(TokenType::LessThanEquals, 12);
        g_operator_precedence.set(TokenType::GreaterThan, 12);
        g_operator_precedence.set(TokenType::GreaterThanEquals, 12);
        g_operator_precedence.set(TokenType::In, 12);
        g_operator_precedence.set(TokenType::Instanceof, 12);

        g_operator_precedence.set(TokenType::EqualsEquals, 11);
        g_operator_precedence.set(TokenType::ExclamationMarkEquals, 11);
        g_operator_precedence.set(TokenType::EqualsEqualsEquals, 11);
        g_operator_precedence.set(TokenType::ExclamationMarkEqualsEquals, 11);

        g_operator_precedence.set(TokenType::Ampersand, 10);

        g_operator_precedence.set(TokenType::Caret, 9);

        g_operator_precedence.set(TokenType::Pipe, 8);

        g_operator_precedence.set(TokenType::DoubleQuestionMark, 7);

        g_operator_precedence.set(TokenType::DoubleAmpersand, 6);

        g_operator_precedence.set(TokenType::DoublePipe, 5);

        g_operator_precedence.set(TokenType::QuestionMark, 4);

        g_operator_precedence.set(TokenType::Equals, 3);
        g_operator_precedence.set(TokenType::PlusEquals, 3);
        g_operator_precedence.set(TokenType::MinusEquals, 3);
        g_operator_precedence.set(TokenType::DoubleAsteriskEquals, 3);
        g_operator_precedence.set(TokenType::AsteriskEquals, 3);
        g_operator_precedence.set(TokenType::SlashEquals, 3);
        g_operator_precedence.set(TokenType::PercentEquals, 3);
        g_operator_precedence.set(TokenType::ShiftLeftEquals, 3);
        g_operator_precedence.set(TokenType::ShiftRightEquals, 3);
        g_operator_precedence.set(TokenType::UnsignedShiftRightEquals, 3);
        g_operator_precedence.set(TokenType::AmpersandEquals, 3);
        g_operator_precedence.set(TokenType::PipeEquals, 3);
        g_operator_precedence.set(TokenType::CaretEquals, 3);

        g_operator_precedence.set(TokenType::Yield, 2);

        g_operator_precedence.set(TokenType::Comma, 1);
    }
}

int Parser::operator_precedence(TokenType type) const
{
    auto it = g_operator_precedence.find(type);
    if (it == g_operator_precedence.end()) {
        dbg() << "JS Parser Error: Non precedence for operator " << Token::name(type);
        ASSERT_NOT_REACHED();
        return -1;
    }

    return it->value;
}

Associativity Parser::operator_associativity(TokenType type) const
{
    switch (type) {
    case TokenType::Period:
    case TokenType::BracketOpen:
    case TokenType::ParenOpen:
    case TokenType::QuestionMarkPeriod:
    case TokenType::Asterisk:
    case TokenType::Slash:
    case TokenType::Percent:
    case TokenType::Plus:
    case TokenType::Minus:
    case TokenType::ShiftLeft:
    case TokenType::ShiftRight:
    case TokenType::UnsignedShiftRight:
    case TokenType::LessThan:
    case TokenType::LessThanEquals:
    case TokenType::GreaterThan:
    case TokenType::GreaterThanEquals:
    case TokenType::In:
    case TokenType::Instanceof:
    case TokenType::EqualsEquals:
    case TokenType::ExclamationMarkEquals:
    case TokenType::EqualsEqualsEquals:
    case TokenType::ExclamationMarkEqualsEquals:
    case TokenType::Typeof:
    case TokenType::Void:
    case TokenType::Delete:
    case TokenType::Ampersand:
    case TokenType::Caret:
    case TokenType::Pipe:
    case TokenType::DoubleQuestionMark:
    case TokenType::DoubleAmpersand:
    case TokenType::DoublePipe:
    case TokenType::Comma:
        return Associativity::Left;
    default:
        return Associativity::Right;
    }
}

NonnullRefPtr<Program> Parser::parse_program()
{
    auto program = create_ast_node<Program>();
    while (!done()) {
        if (match_statement()) {
            program->append(parse_statement());
        } else {
            expected("statement");
            break;
        }
    }

    return program;
}

NonnullRefPtr<Statement> Parser::parse_statement()
{
    switch (m_current_token.type()) {
    case TokenType::Var:
    case TokenType::Let:
    case TokenType::Const:
        return parse_variable_declaration();
    default: {
        if (match_expression()) {
            auto expression = parse_expression(0);
            consume_semicolon();
            return create_ast_node<ExpressionStatement>(expression);
        } else {
            expected("statement");
        }
    }
    }
}

NonnullRefPtr<Expression> Parser::parse_expression(int min_precedence, Associativity associativity)
{
    auto expression = parse_primary_expression();
    while (match_secondary_expression()) {
        int new_precedence = operator_precedence(m_current_token.type());
        if (new_precedence < min_precedence)
            break;
        if (new_precedence == min_precedence && associativity == Associativity::Left)
            break;

        Associativity new_associativity = operator_associativity(m_current_token.type());
        expression = parse_secondary_expression(move(expression), new_precedence, new_associativity);
    }

    return expression;
}

NonnullRefPtr<Expression> Parser::parse_primary_expression()
{
    if (match_unary_expression())
        return parse_unary_expression();

    switch (m_current_token.type()) {
    case TokenType::ParenOpen: {
        consume(TokenType::ParenOpen);
        auto expression = parse_expression(0);
        consume(TokenType::ParenClose);
        return expression;
    }
    case TokenType::NumericLiteral:
        return create_ast_node<NumericLiteral>(consume().double_value());
    case TokenType::BoolLiteral:
        return create_ast_node<BoolLiteral>(consume().bool_value());
    case TokenType::NullLiteral:
        consume();
        return create_ast_node<NullLiteral>();
    case TokenType::StringLiteral:
        return create_ast_node<StringLiteral>(consume().string_value());
    case TokenType::CurlyOpen:
        return parse_object_expression();
    case TokenType::Identifier:
        return create_ast_node<Identifier>(consume().value());
    default:
        expected("primary expression (missing switch case)");
        consume();
    }
}

NonnullRefPtr<Expression> Parser::parse_secondary_expression(NonnullRefPtr<Expression> lhs, int min_precedence, Associativity associativity)
{
    switch (m_current_token.type()) {
    case TokenType::Plus:
        consume();
        return create_ast_node<BinaryExpression>(BinaryOp::Addition, move(lhs), parse_expression(min_precedence, associativity));
    case TokenType::Minus:
        consume();
        return create_ast_node<BinaryExpression>(BinaryOp::Subtraction, move(lhs), parse_expression(min_precedence, associativity));
    case TokenType::Asterisk:
        consume();
        return create_ast_node<BinaryExpression>(BinaryOp::Multiplication, move(lhs), parse_expression(min_precedence, associativity));
    case TokenType::Slash:
        consume();
        return create_ast_node<BinaryExpression>(BinaryOp::Division, move(lhs), parse_expression(min_precedence, associativity));
    case TokenType::Percent:
        consume();
        return create_ast_node<BinaryExpression>(BinaryOp::Modulo, move(lhs), parse_expression(min_precedence, associativity));
    case TokenType::DoubleAsterisk:
        consume();
        return create_ast_node<BinaryExpression>(BinaryOp::Exponentiation, move(lhs), parse_expression(min_precedence, associativity));
    case TokenType::ShiftLeft:
        consume();
        return create_ast_node<BinaryExpression>(BinaryOp::LeftShift, move(lhs), parse_expression(min_precedence, associativity));
    case TokenType::ShiftRight:
        consume();
        return create_ast_node<BinaryExpression>(BinaryOp::RightShift, move(lhs), parse_expression(min_precedence, associativity));
    case TokenType::UnsignedShiftRight:
        consume();
        return create_ast_node<BinaryExpression>(BinaryOp::UnsignedRightShift, move(lhs), parse_expression(min_precedence, associativity));
    case TokenType::EqualsEquals:
        consume();
        return create_ast_node<BinaryExpression>(BinaryOp::Equals, move(lhs), parse_expression(min_precedence, associativity));
    case TokenType::EqualsEqualsEquals:
        consume();
        return create_ast_node<BinaryExpression>(BinaryOp::StrictEquals, move(lhs), parse_expression(min_precedence, associativity));
    case TokenType::LessThan:
        consume();
        return create_ast_node<BinaryExpression>(BinaryOp::LessThan, move(lhs), parse_expression(min_precedence, associativity));
    case TokenType::GreaterThan:
        consume();
        return create_ast_node<BinaryExpression>(BinaryOp::GreaterThan, move(lhs), parse_expression(min_precedence, associativity));
    case TokenType::LessThanEquals:
        consume();
        return create_ast_node<BinaryExpression>(BinaryOp::LessThanOrEquals, move(lhs), parse_expression(min_precedence, associativity));
    case TokenType::GreaterThanEquals:
        consume();
        return create_ast_node<BinaryExpression>(BinaryOp::GreaterThanOrEquals, move(lhs), parse_expression(min_precedence, associativity));
    case TokenType::Ampersand:
        consume();
        return create_ast_node<BinaryExpression>(BinaryOp::BitwiseAnd, move(lhs), parse_expression(min_precedence, associativity));
    case TokenType::Pipe:
        consume();
        return create_ast_node<BinaryExpression>(BinaryOp::BitwiseOr, move(lhs), parse_expression(min_precedence, associativity));
    case TokenType::Caret:
        consume();
        return create_ast_node<BinaryExpression>(BinaryOp::BitwiseXor, move(lhs), parse_expression(min_precedence, associativity));
    case TokenType::DoubleAmpersand:
        consume();
        return create_ast_node<BinaryExpression>(BinaryOp::And, move(lhs), parse_expression(min_precedence, associativity));
    case TokenType::DoublePipe:
        consume();
        return create_ast_node<BinaryExpression>(BinaryOp::Or, move(lhs), parse_expression(min_precedence, associativity));
    case TokenType::Equals:
        consume();
        return parse_assignment_expression(AssignmentOp::Assignment, move(lhs), parse_expression(min_precedence, associativity));
    case TokenType::PlusEquals:
        consume();
        return parse_assignment_expression(AssignmentOp::AdditionAssignment, move(lhs), parse_expression(min_precedence, associativity));
    case TokenType::MinusEquals:
        consume();
        return parse_assignment_expression(AssignmentOp::SubtractionAssignment, move(lhs), parse_expression(min_precedence, associativity));
    case TokenType::AsteriskEquals:
        consume();
        return parse_assignment_expression(AssignmentOp::MultiplicationAssignment, move(lhs), parse_expression(min_precedence, associativity));
    case TokenType::SlashEquals:
        consume();
        return parse_assignment_expression(AssignmentOp::DivisionAssignment, move(lhs), parse_expression(min_precedence, associativity));
    case TokenType::DoubleAsteriskEquals:
        consume();
        return parse_assignment_expression(AssignmentOp::ExponentiationAssignment, move(lhs), parse_expression(min_precedence, associativity));
    case TokenType::PercentEquals:
        consume();
        return parse_assignment_expression(AssignmentOp::ModuloAssignment, move(lhs), parse_expression(min_precedence, associativity));
    case TokenType::AmpersandEquals:
        consume();
        return parse_assignment_expression(AssignmentOp::BitwiseAndAssignment, move(lhs), parse_expression(min_precedence, associativity));
    case TokenType::PipeEquals:
        consume();
        return parse_assignment_expression(AssignmentOp::BitwiseOrAssignment, move(lhs), parse_expression(min_precedence, associativity));
    case TokenType::CaretEquals:
        consume();
        return parse_assignment_expression(AssignmentOp::BitwiseXorAssignment, move(lhs), parse_expression(min_precedence, associativity));
    case TokenType::ShiftLeftEquals:
        consume();
        return parse_assignment_expression(AssignmentOp::LeftShiftAssignment, move(lhs), parse_expression(min_precedence, associativity));
    case TokenType::ShiftRightEquals:
        consume();
        return parse_assignment_expression(AssignmentOp::RightShiftAssignment, move(lhs), parse_expression(min_precedence, associativity));
    case TokenType::UnsignedShiftRightEquals:
        consume();
        return parse_assignment_expression(AssignmentOp::UnsignedRightShiftAssignment, move(lhs), parse_expression(min_precedence, associativity));
    case TokenType::QuestionMark: {
        consume();
        auto consequence = parse_expression(min_precedence, associativity);
        consume(TokenType::Colon);
        auto alternate = parse_expression(min_precedence, associativity);
        return create_ast_node<ConditionalExpression>(move(lhs), move(consequence), move(alternate));
    }
    // Unary postfix increment/decrement
    case TokenType::PlusPlus:
        consume();
        if (!lhs->is_identifier())
            expected("identifier");
        return create_ast_node<UnaryExpression>(UnaryOp::PostfixIncrement, move(lhs));
    case TokenType::MinusMinus:
        consume();
        if (!lhs->is_identifier())
            expected("identifier");
        return create_ast_node<UnaryExpression>(UnaryOp::PostfixDecrement, move(lhs));
    default:
        expected("secondary expression (missing switch case)");
        consume();
    }
}

NonnullRefPtr<AssignmentExpression> Parser::parse_assignment_expression(AssignmentOp op, NonnullRefPtr<Expression> lhs, NonnullRefPtr<Expression> rhs)
{
    if (!lhs->is_identifier())
        expected("identifier");

    return create_ast_node<AssignmentExpression>(op, move(lhs), move(rhs));
}

NonnullRefPtr<UnaryExpression> Parser::parse_unary_expression()
{
    switch (m_current_token.type()) {
    case TokenType::Plus:
        consume();
        return create_ast_node<UnaryExpression>(UnaryOp::Plus, parse_expression(0));
    case TokenType::Minus:
        consume();
        return create_ast_node<UnaryExpression>(UnaryOp::Minus, parse_expression(0));
    case TokenType::Void:
        consume();
        return create_ast_node<UnaryExpression>(UnaryOp::Void, parse_expression(0));
    case TokenType::Typeof:
        consume();
        return create_ast_node<UnaryExpression>(UnaryOp::Typeof, parse_expression(0));
    case TokenType::Tilde:
        consume();
        return create_ast_node<UnaryExpression>(UnaryOp::BitNot, parse_expression(0));
    case TokenType::ExclamationMark:
        consume();
        return create_ast_node<UnaryExpression>(UnaryOp::Not, parse_expression(0));
    case TokenType::PlusPlus: {
        consume();
        auto identifier = parse_expression(0);
        if (!identifier->is_identifier())
            expected("identifier");

        return create_ast_node<UnaryExpression>(UnaryOp::PrefixIncrement, identifier);
    }
    case TokenType::MinusMinus: {
        consume();
        auto identifier = parse_expression(0);
        if (!identifier->is_identifier())
            expected("identifier");

        return create_ast_node<UnaryExpression>(UnaryOp::PrefixDecrement, identifier);
    }
    default:
        expected("unary expression (missing switch case)");
        consume();
    }
}

NonnullRefPtr<VariableDeclaration> Parser::parse_variable_declaration()
{
    Vector<NonnullRefPtr<VariableDeclarator>> declarators;
    DeclarationKind declaration_kind;

    switch (m_current_token.type()) {
    case TokenType::Var:
        consume();
        declaration_kind = DeclarationKind::Var;
        break;
    case TokenType::Let:
        consume();
        declaration_kind = DeclarationKind::Let;
        break;
    case TokenType::Const:
        consume();
        declaration_kind = DeclarationKind::Const;
        break;

    default:
        ASSERT_NOT_REACHED();
    }

    static const int min_precedence = operator_precedence(TokenType::Comma) + 1;

    for (;;) {
        auto id = consume(TokenType::Identifier).value();

        RefPtr<Expression> init;
        if (m_current_token.type() == TokenType::Equals) {
            consume();
            init = parse_expression(min_precedence);
        }

        declarators.append(create_ast_node<VariableDeclarator>(create_ast_node<Identifier>(id), init));

        if (m_current_token.type() == TokenType::Comma)
            consume();
        else
            break;
    }

    consume_semicolon();

    return create_ast_node<VariableDeclaration>(declaration_kind, declarators);
}

NonnullRefPtr<ObjectExpression> Parser::parse_object_expression()
{
    consume(TokenType::CurlyOpen);
    Vector<ObjectProperty> props;
    // TODO: Parse property properly
    auto object = create_ast_node<ObjectExpression>(move(props));
    consume(TokenType::CurlyClose);
    return object;
}

Token Parser::consume(TokenType token)
{
    if (m_current_token.type() != token)
        expected(Token::name(token));
    return consume();
}

Token Parser::consume()
{
    auto old_token = m_current_token;
    m_current_token = m_lexer.next_token();
    return old_token;
}

void Parser::expected(const char* what)
{
    syntax_error(String::format("Unexpected token {}. Expected {}", Token::name(m_current_token.type()), what));
}

void Parser::syntax_error(const String& message, size_t line, size_t column)
{
    if (line == 0 || column == 0) {
        line = m_current_token.line_number();
        column = m_current_token.line_column();
    }

    dbg() << message << " " << line << ':' << column;
    ASSERT_NOT_REACHED();
}

void Parser::consume_semicolon()
{
    if (m_current_token.type() == TokenType::Semicolon) {
        consume();
        return;
    }

    // Assume semicolon if...
    // ...token is preceeded by a new line
    if (m_current_token.trivia().contains('\n'))
        return;

    // ...token match a closing curly bracket
    if (m_current_token.type() == TokenType::CurlyClose)
        return;

    // ...its the last token
    if (m_current_token.type() == TokenType::Eof)
        return;

    expected("semicolon");
}

bool Parser::done() const
{
    return m_current_token.type() == TokenType::Eof;
}

bool Parser::match_statement() const
{
    auto type = m_current_token.type();
    return match_expression()
        || type == TokenType::Var
        || type == TokenType::Let
        || type == TokenType::Const;
}

bool Parser::match_expression() const
{
    auto type = m_current_token.type();
    return match_unary_expression()
        || type == TokenType::ParenOpen
        || type == TokenType::NumericLiteral
        || type == TokenType::BoolLiteral
        || type == TokenType::NullLiteral
        || type == TokenType::StringLiteral
        || type == TokenType::CurlyOpen
        || type == TokenType::Identifier;
}

bool Parser::match_unary_expression() const
{
    auto type = m_current_token.type();
    return type == TokenType::Plus
        || type == TokenType::Minus
        || type == TokenType::Void
        || type == TokenType::Typeof
        || type == TokenType::Tilde
        || type == TokenType::ExclamationMark
        || type == TokenType::PlusPlus
        || type == TokenType::MinusMinus;
}

bool Parser::match_secondary_expression() const
{
    auto type = m_current_token.type();
    return type == TokenType::Plus
        || type == TokenType::Minus
        || type == TokenType::Asterisk
        || type == TokenType::Slash
        || type == TokenType::Percent
        || type == TokenType::DoubleAsterisk
        || type == TokenType::ShiftLeft
        || type == TokenType::ShiftRight
        || type == TokenType::UnsignedShiftRight
        || type == TokenType::EqualsEquals
        || type == TokenType::EqualsEqualsEquals
        || type == TokenType::LessThan
        || type == TokenType::GreaterThan
        || type == TokenType::LessThanEquals
        || type == TokenType::GreaterThanEquals
        || type == TokenType::Ampersand
        || type == TokenType::Pipe
        || type == TokenType::Caret
        || type == TokenType::DoubleAmpersand
        || type == TokenType::DoublePipe
        || type == TokenType::Equals
        || type == TokenType::PlusEquals
        || type == TokenType::MinusEquals
        || type == TokenType::AsteriskEquals
        || type == TokenType::SlashEquals
        || type == TokenType::PercentEquals
        || type == TokenType::DoubleAsteriskEquals
        || type == TokenType::AmpersandEquals
        || type == TokenType::PipeEquals
        || type == TokenType::CaretEquals
        || type == TokenType::ShiftLeftEquals
        || type == TokenType::ShiftRightEquals
        || type == TokenType::UnsignedShiftRightEquals
        || type == TokenType::QuestionMark
        || type == TokenType::PlusPlus
        || type == TokenType::MinusMinus;
}

} // namespace JS
