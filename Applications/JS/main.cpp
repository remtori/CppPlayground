#include <ASL/HookMain.h>
#include <ASL/LogStream.h>
#include <LibJS/AST.h>
#include <LibJS/Interpreter.h>
#include <LibJS/Lexer.h>

ASL_MAIN()
{
    auto interpreter = JS::Interpreter::create();

    {
        auto one = JS::create_ast_node<JS::NumericLiteral>(1);
        auto one_plus_one = JS::create_ast_node<JS::BinaryExpression>(JS::BinaryOp::Plus, one, one);
        auto expression = JS::create_ast_node<JS::UnaryExpression>(JS::UnaryOp::Minus, one_plus_one);
        dbg() << interpreter->run(*expression);
    }

    String source = "(3 + 4) * 5 - 2 / 1";
    JS::Lexer lexer(source);

    JS::Token token = lexer.next_token();
    while (token.type() != JS::TokenType::Eof) {
        dbg() << JS::Token::name(token.type()) << ' ' << token.value();
        token = lexer.next_token();
    }

    return 0;
}
