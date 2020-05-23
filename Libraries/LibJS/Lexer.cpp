#include "Lexer.h"

#include <ASL/HashMap.h>
#include <ctype.h>
#include <stdio.h>

namespace JS {

static HashMap<String, TokenType> g_keywords;
static HashMap<String, TokenType> g_three_char_tokens;
static HashMap<String, TokenType> g_two_char_tokens;
static HashMap<char, TokenType> g_single_char_tokens;

Lexer::Lexer(StringView source)
    : m_source(source)
    , m_current_token(TokenType::Eof, StringView(nullptr), StringView(nullptr), 0, 0)
{
    if (g_keywords.is_empty()) {
        g_keywords.set("await", TokenType::Await);
        g_keywords.set("break", TokenType::Break);
        g_keywords.set("case", TokenType::Case);
        g_keywords.set("catch", TokenType::Catch);
        g_keywords.set("class", TokenType::Class);
        g_keywords.set("const", TokenType::Const);
        g_keywords.set("continue", TokenType::Continue);
        g_keywords.set("debugger", TokenType::Debugger);
        g_keywords.set("default", TokenType::Default);
        g_keywords.set("delete", TokenType::Delete);
        g_keywords.set("do", TokenType::Do);
        g_keywords.set("else", TokenType::Else);
        g_keywords.set("enum", TokenType::Enum);
        g_keywords.set("export", TokenType::Export);
        g_keywords.set("extends", TokenType::Extends);
        g_keywords.set("false", TokenType::BoolLiteral);
        g_keywords.set("finally", TokenType::Finally);
        g_keywords.set("for", TokenType::For);
        g_keywords.set("function", TokenType::Function);
        g_keywords.set("if", TokenType::If);
        g_keywords.set("import", TokenType::Import);
        g_keywords.set("in", TokenType::In);
        g_keywords.set("instanceof", TokenType::Instanceof);
        g_keywords.set("let", TokenType::Let);
        g_keywords.set("new", TokenType::New);
        g_keywords.set("null", TokenType::NullLiteral);
        g_keywords.set("return", TokenType::Return);
        g_keywords.set("super", TokenType::Super);
        g_keywords.set("switch", TokenType::Switch);
        g_keywords.set("this", TokenType::This);
        g_keywords.set("throw", TokenType::Throw);
        g_keywords.set("true", TokenType::BoolLiteral);
        g_keywords.set("try", TokenType::Try);
        g_keywords.set("typeof", TokenType::Typeof);
        g_keywords.set("var", TokenType::Var);
        g_keywords.set("void", TokenType::Void);
        g_keywords.set("while", TokenType::While);
        g_keywords.set("with", TokenType::With);
        g_keywords.set("yield", TokenType::Yield);
    }

    if (g_three_char_tokens.is_empty()) {
        g_three_char_tokens.set("===", TokenType::EqualsEqualsEquals);
        g_three_char_tokens.set("!==", TokenType::ExclamationMarkEqualsEquals);
        g_three_char_tokens.set("**=", TokenType::DoubleAsteriskEquals);
        g_three_char_tokens.set("<<=", TokenType::ShiftLeftEquals);
        g_three_char_tokens.set(">>=", TokenType::ShiftRightEquals);
        g_three_char_tokens.set(">>>", TokenType::UnsignedShiftRight);
        g_three_char_tokens.set("...", TokenType::TripleDot);
    }

    if (g_two_char_tokens.is_empty()) {
        g_two_char_tokens.set("=>", TokenType::Arrow);
        g_two_char_tokens.set("+=", TokenType::PlusEquals);
        g_two_char_tokens.set("-=", TokenType::MinusEquals);
        g_two_char_tokens.set("*=", TokenType::AsteriskEquals);
        g_two_char_tokens.set("/=", TokenType::SlashEquals);
        g_two_char_tokens.set("%=", TokenType::PercentEquals);
        g_two_char_tokens.set("&=", TokenType::AmpersandEquals);
        g_two_char_tokens.set("|=", TokenType::PipeEquals);
        g_two_char_tokens.set("^=", TokenType::CaretEquals);
        g_two_char_tokens.set("&&", TokenType::DoubleAmpersand);
        g_two_char_tokens.set("||", TokenType::DoublePipe);
        g_two_char_tokens.set("??", TokenType::DoubleQuestionMark);
        g_two_char_tokens.set("**", TokenType::DoubleAsterisk);
        g_two_char_tokens.set("==", TokenType::EqualsEquals);
        g_two_char_tokens.set("<=", TokenType::LessThanEquals);
        g_two_char_tokens.set(">=", TokenType::GreaterThanEquals);
        g_two_char_tokens.set("!=", TokenType::ExclamationMarkEquals);
        g_two_char_tokens.set("--", TokenType::MinusMinus);
        g_two_char_tokens.set("++", TokenType::PlusPlus);
        g_two_char_tokens.set("<<", TokenType::ShiftLeft);
        g_two_char_tokens.set(">>", TokenType::ShiftRight);
        g_two_char_tokens.set("?.", TokenType::QuestionMarkPeriod);
    }

    if (g_single_char_tokens.is_empty()) {
        g_single_char_tokens.set('&', TokenType::Ampersand);
        g_single_char_tokens.set('*', TokenType::Asterisk);
        g_single_char_tokens.set('[', TokenType::BracketOpen);
        g_single_char_tokens.set(']', TokenType::BracketClose);
        g_single_char_tokens.set('^', TokenType::Caret);
        g_single_char_tokens.set(':', TokenType::Colon);
        g_single_char_tokens.set(',', TokenType::Comma);
        g_single_char_tokens.set('{', TokenType::CurlyOpen);
        g_single_char_tokens.set('}', TokenType::CurlyClose);
        g_single_char_tokens.set('=', TokenType::Equals);
        g_single_char_tokens.set('!', TokenType::ExclamationMark);
        g_single_char_tokens.set('-', TokenType::Minus);
        g_single_char_tokens.set('(', TokenType::ParenOpen);
        g_single_char_tokens.set(')', TokenType::ParenClose);
        g_single_char_tokens.set('%', TokenType::Percent);
        g_single_char_tokens.set('.', TokenType::Period);
        g_single_char_tokens.set('|', TokenType::Pipe);
        g_single_char_tokens.set('+', TokenType::Plus);
        g_single_char_tokens.set('?', TokenType::QuestionMark);
        g_single_char_tokens.set(';', TokenType::Semicolon);
        g_single_char_tokens.set('/', TokenType::Slash);
        g_single_char_tokens.set('~', TokenType::Tilde);
        g_single_char_tokens.set('<', TokenType::LessThan);
        g_single_char_tokens.set('>', TokenType::GreaterThan);
    }

    // Set the parameters to the correct state
    consume();
}

void Lexer::consume()
{
    if (m_position >= m_source.length()) {
        m_position = m_source.length() + 1;
        m_current_char = EOF;
        return;
    }

    if (m_current_char == '\n') {
        m_line_number++;
        m_line_column = 1;
    } else {
        m_line_column++;
    }

    m_current_char = m_source[m_position++];
}

bool Lexer::match(char a, char b) const
{
    if (m_position + 1 > m_source.length())
        return false;

    return m_current_char == a
        && m_source[m_position] == b;
}

bool Lexer::match(char a, char b, char c) const
{
    if (m_position + 2 > m_source.length())
        return false;

    return m_current_char == a
        && m_source[m_position] == b
        && m_source[m_position + 1] == c;
}

bool Lexer::match(char a, char b, char c, char d) const
{
    if (m_position + 3 > m_source.length())
        return false;

    return m_current_char == a
        && m_source[m_position] == b
        && m_source[m_position + 1] == c
        && m_source[m_position + 2] == d;
}

bool Lexer::is_eof() const
{
    return m_current_char == EOF;
}

bool Lexer::is_identifier_start() const
{
    return isalpha(m_current_char) || m_current_char == '$' || m_current_char == '_';
}

bool Lexer::is_identifier_middle() const
{
    return is_identifier_start() || isdigit(m_current_char);
}

bool Lexer::is_line_comment_start() const
{
    return match('/', '/');
}

bool Lexer::is_block_comment_start() const
{
    return match('/', '*');
}

bool Lexer::is_block_comment_end() const
{
    return match('*', '/');
}

bool Lexer::is_numeric_literal_start() const
{
    return isdigit(m_current_char) || (m_current_char == '.' && m_position + 1 < m_source.length() && isdigit(m_source[m_position + 1]));
}

Token Lexer::next_token()
{
    size_t trivia_start = m_position;

    while (true) {
        if (isspace(m_current_char)) {
            do {
                consume();
            } while (isspace(m_current_char));
        } else if (is_line_comment_start()) {
            do {
                consume();
            } while (!is_eof() && m_current_char != '\n');

        } else if (is_block_comment_start()) {
            do {
                consume();
            } while (!is_eof() && !is_block_comment_end());
            consume(); // consume *
            consume(); // consume /
        } else {
            break;
        }
    }

    size_t value_start = m_position;
    auto token_type = TokenType::Invalid;

    if (is_identifier_start()) {
        do {
            consume();
        } while (is_identifier_middle());

        StringView value = m_source.substring_view(value_start - 1, m_position - value_start);
        auto it = g_keywords.find(value);
        if (it == g_keywords.end()) {
            token_type = TokenType::Identifier;
        } else {
            token_type = it->value;
        }
    } else if (is_numeric_literal_start()) {
        token_type = TokenType::NumericLiteral;

        while (isdigit(m_current_char))
            consume();

        if (m_current_char == '.')
            consume();

        while (isdigit(m_current_char))
            consume();
    } else if (m_current_char == '\'' || m_current_char == '"') {

        char quote = m_current_char;
        consume();

        while (m_current_char != quote && m_current_char != '\n' && !is_eof()) {
            if (m_current_char == '\\')
                consume();

            consume();
        }

        if (m_current_char != quote) {
            token_type = TokenType::UnterminatedStringLiteral;
        } else {
            consume();
            token_type = TokenType::StringLiteral;
        }

    } else if (m_current_char == EOF) {
        token_type = TokenType::Eof;
    } else {
        // There is only one four char operator: >>>=
        bool found_four_char_token = false;
        if (match('>', '>', '>', '=')) {
            found_four_char_token = true;
            consume();
            consume();
            consume();
            consume();
            token_type = TokenType::UnsignedShiftRightEquals;
        }

        bool found_three_char_token = false;
        if (!found_four_char_token && m_position + 1 < m_source.length()) {
            char key[] = { m_current_char, m_source[m_position], m_source[m_position + 1], 0 };
            auto it = g_three_char_tokens.find(key);

            if (it != g_three_char_tokens.end()) {
                found_three_char_token = true;
                consume();
                consume();
                consume();
                token_type = it->value;
            }
        }

        bool found_two_char_token = false;
        if (!found_four_char_token && !found_three_char_token && m_position < m_source.length()) {
            char key[] = { m_current_char, m_source[m_position], 0 };
            auto it = g_two_char_tokens.find(key);

            if (it != g_two_char_tokens.end()) {
                found_two_char_token = true;
                consume();
                consume();
                token_type = it->value;
            }
        }

        bool found_one_char_token = false;
        if (!found_four_char_token && !found_three_char_token && !found_two_char_token) {
            auto it = g_single_char_tokens.find(m_current_char);
            if (it != g_single_char_tokens.end()) {
                found_one_char_token = true;
                consume();
                token_type = it->value;
            }
        }

        if (!found_four_char_token && !found_three_char_token && !found_two_char_token && !found_one_char_token) {
            consume();
            token_type = TokenType::Invalid;
        }
    }

    m_current_token = Token(
        token_type,
        m_source.substring_view(trivia_start - 1, value_start - trivia_start),
        m_source.substring_view(value_start - 1, m_position - value_start),
        m_line_number,
        m_line_column - m_position + value_start);

    return m_current_token;
}

} // namespace JS
