#include "AST.h"

#include "Interpreter.h"
#include "Runtime/Object.h"
#include "Runtime/Value.h"
#include "Runtime/ValueOperators.h"
#include <ASL/LogStream.h>

namespace JS {

Value Program::run(Interpreter& interpreter) const
{
    Value value;
    for (const auto& statement : m_statements) {
        value = statement->run(interpreter);
        dbg() << value;
    }

    return value;
};

Value UnaryExpression::run(Interpreter& interpreter) const
{
    Value value = m_expression->run(interpreter);

    switch (m_op) {
    case UnaryOp::Plus:
        return u_op_plus(value);
    case UnaryOp::Minus:
        return u_op_minus(value);
    case UnaryOp::Void:
        return u_op_void(value);
    case UnaryOp::Typeof:
        return u_op_typeof(value);
    case UnaryOp::BitNot:
        return u_op_bit_not(value);
    case UnaryOp::Not:
        return u_op_not(value);
    case UnaryOp::PrefixIncrement: {
        ASSERT(m_expression->is_identifier());
        auto old_var = value.as_double();
        auto new_var = Value(old_var + 1);
        interpreter.set_variable(static_cast<const Identifier*>(m_expression.ptr())->string(), new_var);
        return new_var;
    }
    case UnaryOp::PrefixDecrement: {
        ASSERT(m_expression->is_identifier());
        auto old_var = value.as_double();
        auto new_var = Value(old_var - 1);
        interpreter.set_variable(static_cast<const Identifier*>(m_expression.ptr())->string(), new_var);
        return new_var;
    }
    case UnaryOp::PostfixIncrement: {
        ASSERT(m_expression->is_identifier());
        auto old_var = value.as_double();
        auto new_var = Value(old_var + 1);
        interpreter.set_variable(static_cast<const Identifier*>(m_expression.ptr())->string(), new_var);
        return value;
    }
    case UnaryOp::PostfixDecrement: {
        ASSERT(m_expression->is_identifier());
        auto old_var = value.as_double();
        auto new_var = Value(old_var - 1);
        interpreter.set_variable(static_cast<const Identifier*>(m_expression.ptr())->string(), new_var);
        return value;
    }

    default:
        return js_undefined();
    }
}

Value BinaryExpression::run(Interpreter& interpreter) const
{
    switch (m_op) {
    case BinaryOp::Addition:
        return b_op_add(m_left->run(interpreter), m_right->run(interpreter));
    case BinaryOp::Subtraction:
        return b_op_sub(m_left->run(interpreter), m_right->run(interpreter));
    case BinaryOp::Multiplication:
        return b_op_mult(m_left->run(interpreter), m_right->run(interpreter));
    case BinaryOp::Division:
        return b_op_div(m_left->run(interpreter), m_right->run(interpreter));
    case BinaryOp::Modulo:
        return b_op_mod(m_left->run(interpreter), m_right->run(interpreter));
    case BinaryOp::Exponentiation:
        return b_op_pow(m_left->run(interpreter), m_right->run(interpreter));
    case BinaryOp::LeftShift:
        return b_op_left_shift(m_left->run(interpreter), m_right->run(interpreter));
    case BinaryOp::RightShift:
        return b_op_right_shift(m_left->run(interpreter), m_right->run(interpreter));
    case BinaryOp::UnsignedRightShift:
        return b_op_unsigned_right_shift(m_left->run(interpreter), m_right->run(interpreter));
    case BinaryOp::Equals:
        return b_op_equals(m_left->run(interpreter), m_right->run(interpreter));
    case BinaryOp::StrictEquals:
        return b_op_strict_equals(m_left->run(interpreter), m_right->run(interpreter));
    case BinaryOp::LessThan:
        return b_op_less_than(m_left->run(interpreter), m_right->run(interpreter));
    case BinaryOp::GreaterThan:
        return b_op_greater_than(m_left->run(interpreter), m_right->run(interpreter));
    case BinaryOp::LessThanOrEquals:
        return b_op_less_than_or_equals(m_left->run(interpreter), m_right->run(interpreter));
    case BinaryOp::GreaterThanOrEquals:
        return b_op_greater_than_or_equals(m_left->run(interpreter), m_right->run(interpreter));
    case BinaryOp::BitwiseAnd:
        return b_op_bit_and(m_left->run(interpreter), m_right->run(interpreter));
    case BinaryOp::BitwiseOr:
        return b_op_bit_or(m_left->run(interpreter), m_right->run(interpreter));
    case BinaryOp::BitwiseXor:
        return b_op_bit_xor(m_left->run(interpreter), m_right->run(interpreter));
    case BinaryOp::And: {
        auto left = m_left->run(interpreter);
        if (!left.to_bool())
            return left;
        return m_right->run(interpreter);
    }
    case BinaryOp::Or: {
        auto left = m_left->run(interpreter);
        if (left.to_bool())
            return left;
        return m_right->run(interpreter);
    }
    default:
        dbg() << "Unknown operator " << (int)m_op;
        ASSERT_NOT_REACHED();
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

Value BoolLiteral::run(Interpreter&) const
{
    return Value(m_value);
}

Value NumericLiteral::run(Interpreter&) const
{
    return Value(m_value);
}

Value StringLiteral::run(Interpreter&) const
{
    return Value(m_string);
}

Value NullLiteral::run(Interpreter&) const
{
    return js_null();
}

Value ObjectExpression::run(Interpreter& interpreter) const
{
    return js_undefined();
}

Value VariableDeclarator::run(Interpreter&) const
{
    // Note: Everything is handled by VariableDeclaration
    ASSERT_NOT_REACHED();
    return js_undefined();
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

    return js_undefined();
}

Value AssignmentExpression::run(Interpreter& interpreter) const
{
    auto value = m_expression->run(interpreter);
    auto var_str = m_identifier->string();

    switch (m_op) {
    case AssignmentOp::Assignment:
        interpreter.set_variable(var_str, value);
        return value;
    case AssignmentOp::AdditionAssignment: {
        auto var = interpreter.get_variable(var_str);
        var = b_op_add(var, value);
        interpreter.set_variable(var_str, var);
        return var;
    }
    case AssignmentOp::SubtractionAssignment: {
        auto var = interpreter.get_variable(var_str);
        var = b_op_sub(var, value);
        interpreter.set_variable(var_str, var);
        return var;
    }
    case AssignmentOp::MultiplicationAssignment: {
        auto var = interpreter.get_variable(var_str);
        var = b_op_mult(var, value);
        interpreter.set_variable(var_str, var);
        return var;
    }
    case AssignmentOp::DivisionAssignment: {
        auto var = interpreter.get_variable(var_str);
        var = b_op_div(var, value);
        interpreter.set_variable(var_str, var);
        return var;
    }
    case AssignmentOp::ModuloAssignment: {
        auto var = interpreter.get_variable(var_str);
        var = b_op_mod(var, value);
        interpreter.set_variable(var_str, var);
        return var;
    }
    case AssignmentOp::ExponentiationAssignment: {
        auto var = interpreter.get_variable(var_str);
        var = b_op_pow(var, value);
        interpreter.set_variable(var_str, var);
        return var;
    }
    case AssignmentOp::BitwiseAndAssignment: {
        auto var = interpreter.get_variable(var_str);
        var = b_op_bit_and(var, value);
        interpreter.set_variable(var_str, var);
        return var;
    }
    case AssignmentOp::BitwiseOrAssignment: {
        auto var = interpreter.get_variable(var_str);
        var = b_op_bit_or(var, value);
        interpreter.set_variable(var_str, var);
        return var;
    }
    case AssignmentOp::BitwiseXorAssignment: {
        auto var = interpreter.get_variable(var_str);
        var = b_op_bit_xor(var, value);
        interpreter.set_variable(var_str, var);
        return var;
    }
    case AssignmentOp::LeftShiftAssignment: {
        auto var = interpreter.get_variable(var_str);
        var = b_op_left_shift(var, value);
        interpreter.set_variable(var_str, var);
        return var;
    }
    case AssignmentOp::RightShiftAssignment: {
        auto var = interpreter.get_variable(var_str);
        var = b_op_right_shift(var, value);
        interpreter.set_variable(var_str, var);
        return var;
    }
    case AssignmentOp::UnsignedRightShiftAssignment: {
        auto var = interpreter.get_variable(var_str);
        var = b_op_unsigned_right_shift(var, value);
        interpreter.set_variable(var_str, var);
        return var;
    }
    }
    return value;
}

Value ExpressionStatement::run(Interpreter& interpreter) const
{
    return m_expression->run(interpreter);
}

Value ConditionalExpression::run(Interpreter& interpreter) const
{
    if (m_test->run(interpreter).to_bool())
        return m_consequent->run(interpreter);
    else
        return m_alternate->run(interpreter);
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
        printf("Plus");
        break;
    case UnaryOp::Minus:
        printf("Minus");
        break;
    case UnaryOp::Void:
        printf("Void");
        break;
    case UnaryOp::Typeof:
        printf("Typeof");
        break;
    case UnaryOp::BitNot:
        printf("BitNot");
        break;
    case UnaryOp::Not:
        printf("Not");
        break;
    case UnaryOp::PostfixIncrement:
        printf("PostfixIncrement");
        break;
    case UnaryOp::PostfixDecrement:
        printf("PostfixDecrement");
        break;
    case UnaryOp::PrefixIncrement:
        printf("PrefixIncrement");
        break;
    case UnaryOp::PrefixDecrement:
        printf("PrefixIncrement");
        break;
    }

    m_expression->dump(indent + 2);
}

void BinaryExpression::dump(int indent) const
{
    ASTNode::dump(indent);

    switch (m_op) {
    case BinaryOp::Addition:
        printf("Addition");
        break;
    case BinaryOp::Subtraction:
        printf("Subtraction");
        break;
    case BinaryOp::Multiplication:
        printf("Multiplication");
        break;
    case BinaryOp::Division:
        printf("Division");
        break;
    case BinaryOp::Modulo:
        printf("Modulo");
        break;
    case BinaryOp::Exponentiation:
        printf("Exponentiation");
        break;
    case BinaryOp::LeftShift:
        printf("LeftShift");
        break;
    case BinaryOp::RightShift:
        printf("RightShift");
        break;
    case BinaryOp::UnsignedRightShift:
        printf("UnsignedRightShift");
        break;
    case BinaryOp::Equals:
        printf("Equals");
        break;
    case BinaryOp::StrictEquals:
        printf("StrictEquals");
        break;
    case BinaryOp::LessThan:
        printf("LessThan");
        break;
    case BinaryOp::GreaterThan:
        printf("GreaterThan");
        break;
    case BinaryOp::LessThanOrEquals:
        printf("LessThanOrEquals");
        break;
    case BinaryOp::GreaterThanOrEquals:
        printf("GreaterThanOrEquals");
        break;
    case BinaryOp::BitwiseAnd:
        printf("BitwiseAnd");
        break;
    case BinaryOp::BitwiseOr:
        printf("BitwiseOr");
        break;
    case BinaryOp::BitwiseXor:
        printf("BitwiseXor");
        break;
    case BinaryOp::And:
        printf("And");
        break;
    case BinaryOp::Or:
        printf("Or");
        break;
    }

    m_left->dump(indent + 2);
    m_right->dump(indent + 2);
}

void BoolLiteral::dump(int indent) const
{
    ASTNode::dump(indent);
    printf(m_value ? "true" : "false");
}

void NumericLiteral::dump(int indent) const
{
    ASTNode::dump(indent);
    printf("%lf", m_value);
}

void StringLiteral::dump(int indent) const
{
    ASTNode::dump(indent);
    printf("`%s`", m_string.characters());
}

void ObjectExpression::dump(int indent) const
{
    ASTNode::dump(indent);
    for (auto& prop : m_properties) {
        space(indent);
        printf("Key Value:");
        prop.key->dump(indent + 2);
        prop.value->dump(indent + 2);
    }
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

void ConditionalExpression::dump(int indent) const
{
    ASTNode::dump(indent);
    space(indent + 1);
    printf("test:");
    m_test->dump(indent + 4);

    space(indent + 1);
    printf("consequent:");
    m_consequent->dump(indent + 4);

    space(indent + 1);
    printf("alternate:");
    m_alternate->dump(indent + 4);
}

} // namespace JS