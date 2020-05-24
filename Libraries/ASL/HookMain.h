#pragma once

#include <ASL/Backtrace.h>
#include <ASL/LogStream.h>

#define ASL_MAIN()                              \
    int __user_defined_main(int, char**);       \
    int main(int argc, char** argv)             \
    {                                           \
        HANDLE_CRASH()                          \
        ASL::set_process_name(argc, argv);      \
        return __user_defined_main(argc, argv); \
    }                                           \
    int __user_defined_main(int argc, char** argv)
