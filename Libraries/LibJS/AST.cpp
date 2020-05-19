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

} // namespace JS