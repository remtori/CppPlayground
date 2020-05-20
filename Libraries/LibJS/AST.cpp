#include "AST.h"

#include "Interpreter.h"
#include <ASL/LogStream.h>

namespace JS {

Value UnaryExpression::run(Interpreter& interpreter) const
{
    Value value = m_expression->run(interpreter);
    switch (m_op) {
    case UnaryOp::Plus:
        return value;
    case UnaryOp::Minus:
        return -value;
    default:
        return 0;
    }
}

Value BinaryExpression::run(Interpreter& interpreter) const
{
    Value left = m_left->run(interpreter);
    Value right = m_right->run(interpreter);

    switch (m_op) {
    case BinaryOp::Plus:
        return left + right;
    case BinaryOp::Minus:
        return left - right;
    case BinaryOp::Mult:
        return left * right;
    case BinaryOp::Div:
        return left / right;
    default:
        return 0;
    }
}

Value Identifier::run(Interpreter& interpreter) const
{
    auto value = interpreter.get_variable(string());
    // if (!value) {
    //     dbg() << "Identifier '" << string() << "' not known";
    //     ASSERT_NOT_REACHED();
    // }

    return value;
}

Value VariableDeclarator::run(Interpreter& interpreter) const
{
    // Note: Everything is handled by VariableDeclaration
    ASSERT_NOT_REACHED();
    return 0;
}

Value VariableDeclaration::run(Interpreter& interpreter) const
{
    for (auto& declarator : m_declarations) {
        if (auto* init = declarator->init()) {
            auto initializer_result = init->run(interpreter);
            auto variable_name = declarator->id().string();
            interpreter.set_variable(variable_name, initializer_result);
        }
    }

    return 0;
}

Value AssignmentExpression::run(Interpreter& interpreter) const
{
    auto value = m_expression->run(interpreter);
    interpreter.set_variable(m_identifier->string(), value);
    return value;
}

Value ExpressionStatement::run(Interpreter& interpreter) const
{
    return m_expression->run(interpreter);
}

void space(int indent)
{
    putchar('\n');
    while (--indent > 0)
        putchar(' ');
}

void ASTNode::dump(int indent) const
{
    space(indent);
    printf("%s ", class_name());
}

void Program::dump(int indent) const
{
    ASTNode::dump(indent);
    for (const auto& statement : m_statements)
        statement->dump(indent + 2);
}

void UnaryExpression::dump(int indent) const
{
    ASTNode::dump(indent);

    switch (m_op) {
    case UnaryOp::Plus:
        putchar('+');
        break;
    case UnaryOp::Minus:
        putchar('-');
        break;
    }

    m_expression->dump(indent + 2);
}

void BinaryExpression::dump(int indent) const
{
    ASTNode::dump(indent);

    switch (m_op) {
    case BinaryOp::Plus:
        putchar('+');
        break;
    case BinaryOp::Minus:
        putchar('-');
        break;
    case BinaryOp::Mult:
        putchar('*');
        break;
    case BinaryOp::Div:
        putchar('/');
        break;
    }

    m_left->dump(indent + 2);
    m_right->dump(indent + 2);
}

void NumericLiteral::dump(int indent) const
{
    ASTNode::dump(indent);
    printf("%lf", m_value);
}

void Identifier::dump(int indent) const
{
    ASTNode::dump(indent);
    printf(" \"%s\"", m_string.characters());
}

void VariableDeclarator::dump(int indent) const
{
    ASTNode::dump(indent);
    m_id->dump(indent + 2);
    if (!m_init.is_null())
        m_init->dump(indent + 2);
}

void VariableDeclaration::dump(int indent) const
{
    ASTNode::dump(indent);
    switch (m_declaration_kind) {
    case DeclarationKind::Const:
        printf("const");
        break;
    case DeclarationKind::Let:
        printf("let");
        break;
    case DeclarationKind::Var:
        printf("var");
        break;
    }

    for (const auto& d : m_declarations)
        d->dump(indent + 2);
}

void AssignmentExpression::dump(int indent) const
{
    ASTNode::dump(indent);
    m_identifier->dump(indent + 2);
    m_expression->dump(indent + 2);
}

void ExpressionStatement::dump(int indent) const
{
    ASTNode::dump(indent);
    m_expression->dump(indent + 2);
}

} // namespace JS