#pragma once

#include <ASL/Platform.h>

#ifdef PLATFORM_LINUX
#    include <signal.h>
#elif defined(PLATFORM_WINDOWS)
#endif

#define HANDLE_CRASH()                     \
    signal(SIGABRT, ASL::print_backtrace); \
    signal(SIGSEGV, ASL::print_backtrace); \
    signal(SIGTRAP, ASL::print_backtrace);

namespace ASL {
void print_backtrace(int signal);
} // namespace ASL
