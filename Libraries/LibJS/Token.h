#pragma once

#include <ASL/HashFunctions.h>
#include <ASL/String.h>
#include <ASL/Traits.h>

namespace JS {

#define _FOR_EVERY_JS_TOKEN              \
    __TOKEN(Ampersand)                   \
    __TOKEN(AmpersandEquals)             \
    __TOKEN(Arrow)                       \
    __TOKEN(Asterisk)                    \
    __TOKEN(AsteriskEquals)              \
    __TOKEN(Async)                       \
    __TOKEN(Await)                       \
    __TOKEN(BoolLiteral)                 \
    __TOKEN(BracketClose)                \
    __TOKEN(BracketOpen)                 \
    __TOKEN(Break)                       \
    __TOKEN(Caret)                       \
    __TOKEN(CaretEquals)                 \
    __TOKEN(Case)                        \
    __TOKEN(Catch)                       \
    __TOKEN(Class)                       \
    __TOKEN(Colon)                       \
    __TOKEN(Comma)                       \
    __TOKEN(Const)                       \
    __TOKEN(Continue)                    \
    __TOKEN(CurlyClose)                  \
    __TOKEN(CurlyOpen)                   \
    __TOKEN(Debugger)                    \
    __TOKEN(Default)                     \
    __TOKEN(Delete)                      \
    __TOKEN(Do)                          \
    __TOKEN(DoubleAmpersand)             \
    __TOKEN(DoubleAsterisk)              \
    __TOKEN(DoubleAsteriskEquals)        \
    __TOKEN(DoublePipe)                  \
    __TOKEN(DoubleQuestionMark)          \
    __TOKEN(Else)                        \
    __TOKEN(Enum)                        \
    __TOKEN(Eof)                         \
    __TOKEN(Equals)                      \
    __TOKEN(EqualsEquals)                \
    __TOKEN(EqualsEqualsEquals)          \
    __TOKEN(ExclamationMark)             \
    __TOKEN(ExclamationMarkEquals)       \
    __TOKEN(ExclamationMarkEqualsEquals) \
    __TOKEN(Export)                      \
    __TOKEN(Extends)                     \
    __TOKEN(Finally)                     \
    __TOKEN(For)                         \
    __TOKEN(Function)                    \
    __TOKEN(GreaterThan)                 \
    __TOKEN(GreaterThanEquals)           \
    __TOKEN(Identifier)                  \
    __TOKEN(If)                          \
    __TOKEN(Implements)                  \
    __TOKEN(Import)                      \
    __TOKEN(In)                          \
    __TOKEN(Instanceof)                  \
    __TOKEN(Interface)                   \
    __TOKEN(Invalid)                     \
    __TOKEN(LessThan)                    \
    __TOKEN(LessThanEquals)              \
    __TOKEN(Let)                         \
    __TOKEN(Minus)                       \
    __TOKEN(MinusEquals)                 \
    __TOKEN(MinusMinus)                  \
    __TOKEN(New)                         \
    __TOKEN(NullLiteral)                 \
    __TOKEN(NumericLiteral)              \
    __TOKEN(Package)                     \
    __TOKEN(ParenClose)                  \
    __TOKEN(ParenOpen)                   \
    __TOKEN(Percent)                     \
    __TOKEN(PercentEquals)               \
    __TOKEN(Period)                      \
    __TOKEN(Pipe)                        \
    __TOKEN(PipeEquals)                  \
    __TOKEN(Plus)                        \
    __TOKEN(PlusEquals)                  \
    __TOKEN(PlusPlus)                    \
    __TOKEN(Private)                     \
    __TOKEN(Protected)                   \
    __TOKEN(Public)                      \
    __TOKEN(QuestionMark)                \
    __TOKEN(QuestionMarkPeriod)          \
    __TOKEN(RegexLiteral)                \
    __TOKEN(Return)                      \
    __TOKEN(Semicolon)                   \
    __TOKEN(ShiftLeft)                   \
    __TOKEN(ShiftLeftEquals)             \
    __TOKEN(ShiftRight)                  \
    __TOKEN(ShiftRightEquals)            \
    __TOKEN(Slash)                       \
    __TOKEN(SlashEquals)                 \
    __TOKEN(Static)                      \
    __TOKEN(StringLiteral)               \
    __TOKEN(Super)                       \
    __TOKEN(Switch)                      \
    __TOKEN(TemplateLiteralEnd)          \
    __TOKEN(TemplateLiteralExprEnd)      \
    __TOKEN(TemplateLiteralExprStart)    \
    __TOKEN(TemplateLiteralStart)        \
    __TOKEN(TemplateLiteralString)       \
    __TOKEN(This)                        \
    __TOKEN(Throw)                       \
    __TOKEN(Tilde)                       \
    __TOKEN(TripleDot)                   \
    __TOKEN(Try)                         \
    __TOKEN(Typeof)                      \
    __TOKEN(UnsignedShiftRight)          \
    __TOKEN(UnsignedShiftRightEquals)    \
    __TOKEN(UnterminatedStringLiteral)   \
    __TOKEN(UnterminatedTemplateLiteral) \
    __TOKEN(Var)                         \
    __TOKEN(Void)                        \
    __TOKEN(While)                       \
    __TOKEN(With)                        \
    __TOKEN(Yield)

enum class TokenType {
#define __TOKEN(x) x,
    _FOR_EVERY_JS_TOKEN
#undef __TOKEN
};

class Token {
public:
    static const char* name(TokenType);

    Token(TokenType type, StringView trivia, StringView value, size_t line_number, size_t line_column)
        : m_type(type)
        , m_trivia(trivia)
        , m_value(value)
        , m_line_number(line_number)
        , m_line_column(line_column)
    {
    }

    TokenType type() const { return m_type; }
    const char* name() const { return name(m_type); }

    const StringView& trivia() const { return m_trivia; }
    const StringView& value() const { return m_value; }
    size_t line_number() const { return m_line_number; }
    size_t line_column() const { return m_line_column; }
    double double_value() const;
    bool bool_value() const;
    String string_value() const;

    bool is_identifier_name() const;

private:
    TokenType m_type;
    StringView m_trivia;
    StringView m_value;
    size_t m_line_number;
    size_t m_line_column;
};

} // namespace JS

namespace ASL {

template<>
struct Traits<JS::TokenType> : public GenericTraits<JS::TokenType> {
    static constexpr bool is_trivial() { return true; }
    static unsigned hash(JS::TokenType t) { return int_hash((int)t); }
};

} // namespace ASL
