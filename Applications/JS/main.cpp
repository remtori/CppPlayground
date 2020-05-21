#include <ASL/File.h>
#include <ASL/HookMain.h>
#include <ASL/LogStream.h>
#include <LibJS/AST.h>
#include <LibJS/Interpreter.h>
#include <LibJS/Lexer.h>
#include <LibJS/Parser.h>

ASL_MAIN()
{
    auto interpreter = JS::Interpreter::create();

    if (argc < 2) {
        argv[1] = (char*)"./Applications/JS/simple.js";
    }

    File source_file(argv[1]);
    if (!source_file.open(File::Read)) {
        dbg() << "Can not open file " << source_file.filename();
        return 0;
    }

    String source(source_file.read_all());
    StringView source_view = source.view();

    JS::Parser parser(source_view);
    auto program = parser.parse_program();

    if (argc > 2) {
        program->dump(0);
        printf("\n");
    }

    dbg() << interpreter->run(*program);

    return 0;
}
