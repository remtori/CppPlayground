#pragma once

#include <ASL/NonnullRefPtr.h>
#include <ASL/RefCounted.h>
#include <ASL/SharedString.h>
#include <ASL/String.h>
#include <ASL/Types.h>
#include <LibJS/Forward.h>

namespace JS {

template<class T, typename... Args>
static inline NonnullRefPtr<T> create_ast_node(Args&&... args)
{
    return adopt(*new T(forward<Args>(args)...));
}

class ASTNode : public RefCounted<ASTNode> {
public:
    virtual ~ASTNode() {}
    virtual Value run(Interpreter&) const = 0;
    virtual const char* class_name() const = 0;
    virtual void dump(int indent) const;

    virtual bool is_identifier() const { return false; }
    virtual bool is_program() const { return false; }
};

class Expression : public ASTNode {
};

class Statement : public ASTNode {
};

class Declaration : public Statement {
};

class ScopeNode : public Statement {
public:
    ScopeNode() {}

    void append(NonnullRefPtr<Statement> statement) { m_statements.append(move(statement)); }

    const Vector<NonnullRefPtr<Statement>> statements() const { return m_statements; };

    virtual Value run(Interpreter& interpreter) const override;
    virtual const char* class_name() const override { return "ScopeNode"; }
    void dump(int indent) const override;

private:
    Vector<NonnullRefPtr<Statement>> m_statements;
};

class Program final : public ScopeNode {
public:
    Program() {}

    virtual bool is_program() const { return true; }

    virtual const char* class_name() const override { return "Program"; }
};

enum class UnaryOp {
    Plus,
    Minus,
    Void,
    Typeof,
    BitNot,
    Not,
    PrefixIncrement,
    PrefixDecrement,
    PostfixIncrement,
    PostfixDecrement,
};

class UnaryExpression final : public Expression {
public:
    UnaryExpression(UnaryOp op, NonnullRefPtr<Expression> expression)
        : m_op(op)
        , m_expression(expression)
    {
    }

    virtual Value run(Interpreter&) const override;
    virtual const char* class_name() const override { return "UnaryExpression"; }
    void dump(int indent) const override;

private:
    UnaryOp m_op;
    NonnullRefPtr<Expression> m_expression;
};

enum class BinaryOp {
    Addition,
    Subtraction,
    Multiplication,
    Division,
    Modulo,
    Exponentiation,
    LeftShift,
    RightShift,
    UnsignedRightShift,
    Equals,
    StrictEquals,
    LessThan,
    GreaterThan,
    LessThanOrEquals,
    GreaterThanOrEquals,
    BitwiseAnd,
    BitwiseOr,
    BitwiseXor,
    And,
    Or,
};

class BinaryExpression final : public Expression {
public:
    BinaryExpression(BinaryOp op, NonnullRefPtr<Expression> left, NonnullRefPtr<Expression> right)
        : m_op(op)
        , m_left(left)
        , m_right(right)
    {
    }

    virtual Value run(Interpreter&) const override;
    virtual const char* class_name() const override { return "BinaryExpression"; }
    void dump(int indent) const override;

private:
    BinaryOp m_op;
    NonnullRefPtr<Expression> m_left;
    NonnullRefPtr<Expression> m_right;
};

class Literal : public Expression {
};

class BoolLiteral final : public Literal {
public:
    explicit BoolLiteral(bool value)
        : m_value(value)
    {
    }

    bool value() const { return m_value; }

    virtual Value run(Interpreter&) const override;
    virtual const char* class_name() const override { return "BoolLiteral"; }
    void dump(int indent) const override;

private:
    bool m_value;
};

class NumericLiteral final : public Literal {
public:
    explicit NumericLiteral(double value)
    {
        m_value = value;
    }

    double value() const { return m_value; }

    virtual Value run(Interpreter&) const override;
    virtual const char* class_name() const override { return "NumericLiteral"; }
    void dump(int indent) const override;

private:
    double m_value = 0;
};

class StringLiteral : public Literal {
public:
    explicit StringLiteral(const SharedString& string)
        : m_string(string)
    {
    }

    virtual Value run(Interpreter&) const override;
    virtual const char* class_name() const override { return "StringLiteral"; }
    void dump(int indent) const override;

private:
    SharedString m_string;
};

class NullLiteral : public Literal {
public:
    NullLiteral() {}

    virtual Value run(Interpreter&) const override;
    virtual const char* class_name() const override { return "NullLiteral"; }
};

class Identifier final : public Expression {
public:
    explicit Identifier(const SharedString& str)
        : m_string(str)
    {
    }

    const SharedString& string() const { return m_string; }

    virtual Value run(Interpreter&) const override;
    virtual const char* class_name() const override { return "Identifier"; }
    void dump(int indent) const override;

    bool is_identifier() const override { return true; }

private:
    SharedString m_string;
};

class VariableDeclarator final : public Expression {
public:
    explicit VariableDeclarator(NonnullRefPtr<Identifier> id, RefPtr<Expression> init = nullptr)
        : m_id(id)
        , m_init(init)
    {
    }

    const Identifier& id() const { return *m_id; }
    const Expression* init() const { return m_init.ptr(); }

    Value run(Interpreter&) const override;
    const char* class_name() const override { return "VariableDeclarator"; }
    void dump(int indent) const override;

private:
    NonnullRefPtr<Identifier> m_id;
    RefPtr<Expression> m_init;
};

enum class DeclarationKind {
    Var,
    Let,
    Const,
};

class VariableDeclaration : public Declaration {
public:
    explicit VariableDeclaration(DeclarationKind declaration_kind, Vector<NonnullRefPtr<VariableDeclarator>> declarators)
        : m_declaration_kind(declaration_kind)
        , m_declarations(move(declarators))
    {
    }

    DeclarationKind declaration_kind() const { return m_declaration_kind; }
    const Vector<NonnullRefPtr<VariableDeclarator>>& declarations() const { return m_declarations; }

    virtual Value run(Interpreter&) const override;
    virtual const char* class_name() const override { return "VariableDeclaration"; }
    virtual void dump(int indent) const override;

private:
    DeclarationKind m_declaration_kind;
    Vector<NonnullRefPtr<VariableDeclarator>> m_declarations;
};

enum class AssignmentOp {
    Assignment,
    AdditionAssignment,
    SubtractionAssignment,
    MultiplicationAssignment,
    DivisionAssignment,
    ModuloAssignment,
    ExponentiationAssignment,
    BitwiseAndAssignment,
    BitwiseOrAssignment,
    BitwiseXorAssignment,
    LeftShiftAssignment,
    RightShiftAssignment,
    UnsignedRightShiftAssignment,
};

class AssignmentExpression final : public Expression {
public:
    explicit AssignmentExpression(AssignmentOp op, NonnullRefPtr<Identifier> identifier, NonnullRefPtr<Expression> expression)
        : m_op(op)
        , m_identifier(identifier)
        , m_expression(expression)
    {
    }

    const Identifier& identifier() const { return *m_identifier; }
    const Expression* expression() const { return m_expression.ptr(); }

    Value run(Interpreter&) const override;
    const char* class_name() const override { return "AssignmentExpression"; }
    void dump(int indent) const override;

private:
    AssignmentOp m_op;
    NonnullRefPtr<Identifier> m_identifier;
    NonnullRefPtr<Expression> m_expression;
};

class ExpressionStatement : public Statement {
public:
    explicit ExpressionStatement(NonnullRefPtr<Expression> expression)
        : m_expression(expression)
    {
    }

    Value run(Interpreter&) const override;
    const char* class_name() const override { return "ExpressionStatement"; }
    void dump(int indent) const override;

private:
    NonnullRefPtr<Expression> m_expression;
};

class ConditionalExpression : public Expression {
public:
    explicit ConditionalExpression(NonnullRefPtr<Expression> test, NonnullRefPtr<Expression> consequent, NonnullRefPtr<Expression> alternate)
        : m_test(test)
        , m_consequent(consequent)
        , m_alternate(alternate)
    {
    }

    Value run(Interpreter&) const override;
    const char* class_name() const override { return "ConditionalExpression"; }
    void dump(int indent) const override;

private:
    NonnullRefPtr<Expression> m_test;
    NonnullRefPtr<Expression> m_consequent;
    NonnullRefPtr<Expression> m_alternate;
};

struct ObjectProperty {
    NonnullRefPtr<Identifier> key;
    NonnullRefPtr<Expression> value;
};

class ObjectExpression : public Expression {
public:
    explicit ObjectExpression(Vector<ObjectProperty> properties)
        : m_properties(move(properties))
    {
    }

    const Vector<ObjectProperty>& properties() const { return m_properties; }

    virtual Value run(Interpreter&) const override;
    virtual const char* class_name() const override { return "ObjectExpression"; }
    virtual void dump(int) const override;

private:
    Vector<ObjectProperty> m_properties;
};

} // namespace JS
