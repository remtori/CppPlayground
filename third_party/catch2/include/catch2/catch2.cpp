#define CATCH_CONFIG_RUNNER
#include "catch2.hpp"

#include <ASL/HookMain.h>

ASL_MAIN()
{
    int result = Catch::Session().run(argc, argv);
    return result;
}
