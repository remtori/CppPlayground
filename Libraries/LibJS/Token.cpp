#include "Token.h"

#include <ASL/StringUtils.h>

namespace JS {

const char* Token::name(TokenType token)
{
    switch (token) {
#define __TOKEN(x)     \
    case TokenType::x: \
        return #x;
        _FOR_EVERY_JS_TOKEN
#undef __TOKEN
    }

    return "<Unknown JS Token>";
}

double Token::double_value() const
{
    ASSERT(m_type == TokenType::NumericLiteral);
    return ASL::string_to_double(m_value);
}

bool Token::bool_value() const
{
    ASSERT(m_type == TokenType::BoolLiteral);
    return m_value == "true";
}

String Token::string_value() const
{
    ASSERT(m_type == TokenType::StringLiteral || m_type == TokenType::TemplateLiteralString);
    return m_value;
}

bool Token::is_identifier_name() const
{
    // IdentifierNames are Identifiers + ReservedWords
    // The standard defines this reversed: Identifiers are IdentifierNames except reserved words
    // https://www.ecma-international.org/ecma-262/5.1/#sec-7.6
    return m_type == TokenType::Identifier
        || m_type == TokenType::Await
        || m_type == TokenType::BoolLiteral
        || m_type == TokenType::Break
        || m_type == TokenType::Case
        || m_type == TokenType::Catch
        || m_type == TokenType::Class
        || m_type == TokenType::Const
        || m_type == TokenType::Continue
        || m_type == TokenType::Default
        || m_type == TokenType::Delete
        || m_type == TokenType::Do
        || m_type == TokenType::Else
        || m_type == TokenType::Finally
        || m_type == TokenType::For
        || m_type == TokenType::Function
        || m_type == TokenType::If
        || m_type == TokenType::In
        || m_type == TokenType::Instanceof
        || m_type == TokenType::Interface
        || m_type == TokenType::Let
        || m_type == TokenType::New
        || m_type == TokenType::NullLiteral
        || m_type == TokenType::Return
        || m_type == TokenType::Switch
        || m_type == TokenType::This
        || m_type == TokenType::Throw
        || m_type == TokenType::Try
        || m_type == TokenType::Typeof
        || m_type == TokenType::Var
        || m_type == TokenType::Void
        || m_type == TokenType::While
        || m_type == TokenType::Yield;
}

} // namespace JS
