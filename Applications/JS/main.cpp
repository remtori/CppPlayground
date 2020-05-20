#include <ASL/HookMain.h>
#include <ASL/LogStream.h>
#include <LibJS/AST.h>
#include <LibJS/Interpreter.h>
#include <LibJS/Lexer.h>
#include <LibJS/Parser.h>

ASL_MAIN()
{
    String source = "(4 + 3) * 5 - 2 / 2 * 3";
    StringView source_view = source.view();

    auto interpreter = JS::Interpreter::create();

    // JS::Lexer lexer(source);
    // JS::Token token = lexer.next_token();
    // while (token.type() != JS::TokenType::Eof) {
    //     dbg() << JS::Token::name(token.type()) << ' ' << token.value();
    //     token = lexer.next_token();
    // }

    JS::Parser parser(source_view);
    auto program = parser.parse_program();
    dbg() << interpreter->run(*program);
    program->dump(0);

    return 0;
}
