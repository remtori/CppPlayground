#include "AST.h"

#include "Interpreter.h"
#include <ASL/LogStream.h>

namespace JS {

JSValue UnaryExpression::run(Interpreter& interpreter)
{
    JSValue value = m_expression->run(interpreter);
    switch (m_op) {
    case UnaryOp::Plus:
        return value;
    case UnaryOp::Minus:
        return -value;
    default:
        return 0;
    }
}

JSValue BinaryExpression::run(Interpreter& interpreter)
{
    JSValue left = m_left->run(interpreter);
    JSValue right = m_right->run(interpreter);

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

void space(int indent)
{
    putchar('\n');
    while (--indent > 0)
        putchar(' ');
}

void ASTNode::dump(int indent)
{
    space(indent);
    printf("%s ", class_name());
}

void Program::dump(int indent)
{
    ASTNode::dump(indent);
    m_body->dump(indent + 2);
}

void UnaryExpression::dump(int indent)
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

void BinaryExpression::dump(int indent)
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

void NumericLiteral::dump(int indent)
{
    ASTNode::dump(indent);
    printf("%lf", m_value);
}

} // namespace JS