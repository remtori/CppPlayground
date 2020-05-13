#pragma once

#include "Forward.h"

#include <ASL/NonnullRefPtr.h>
#include <ASL/RefCounted.h>
#include <ASL/String.h>
#include <ASL/Types.h>

namespace JS {

class ASTNode : public RefCounted<ASTNode> {
public:
    virtual ~ASTNode() {}
    virtual JSValue run(Interpreter&) = 0;
    virtual const char* class_name() = 0;
};

class Expression : public ASTNode {
};

class Program final : public Expression {
public:
    Program();

    void add_body(RefPtr<Expression> body)
    {
        m_body = body;
    }

    void add_error(const String& err)
    {
        m_error = err;
    }

    bool has_error() const { return !m_error.is_empty(); }

    virtual JSValue run(Interpreter& interpreter) override
    {
        m_body->run(interpreter);
    };

    virtual const char* class_name() override { return "Program"; }

private:
    RefPtr<Expression> m_body;
    String m_error;
};

enum class UnaryOp {
    Plus,
    Minus,
};

class UnaryExpression final : public Expression {
public:
    UnaryExpression(UnaryOp op, NonnullRefPtr<Expression> expression)
        : m_op(op)
        , m_expression(expression)
    {
    }

    virtual JSValue run(Interpreter&) override;
    virtual const char* class_name() override { return "UnaryExpression"; }

private:
    UnaryOp m_op;
    NonnullRefPtr<Expression> m_expression;
};

enum class BinaryOp {
    Plus,
    Minus,
    Mult,
    Div,
};

class BinaryExpression final : public Expression {
public:
    BinaryExpression(BinaryOp op, NonnullRefPtr<Expression> left, NonnullRefPtr<Expression> right)
        : m_op(op)
        , m_left(left)
        , m_right(right)
    {
    }

    virtual JSValue run(Interpreter&) override;
    virtual const char* class_name() override { return "BinaryExpression"; }

private:
    BinaryOp m_op;
    NonnullRefPtr<Expression> m_left;
    NonnullRefPtr<Expression> m_right;
};

class Literal : public Expression {
};

class NumericLiteral final : public Literal {
public:
    explicit NumericLiteral(double value)
    {
        m_value = value;
    }

    virtual JSValue run(Interpreter&) override { return m_value; }

    virtual const char* class_name() override { return "NumericLiteral"; }

private:
    double m_value = 0;
};

} // namespace JS
