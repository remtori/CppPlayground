#pragma once

#include "LogStream.h"

#define ASL_MAIN()                              \
    int __user_defined_main(int, char**);       \
    int main(int argc, char** argv)             \
    {                                           \
        ASL::set_process_name(argc, argv);      \
        return __user_defined_main(argc, argv); \
    }                                           \
    int __user_defined_main(int argc, char** argv)
