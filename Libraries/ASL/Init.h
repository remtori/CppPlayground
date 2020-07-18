#pragma once

#include <ASL/Backtrace.h>
#include <ASL/Logger.h>
#include <ASL/Platform.h>

#ifdef PLATFORM_LINUX
#    include <signal.h>
#elif defined(PLATFORM_WINDOWS)
#endif

namespace ASL {

void initialize(const char* arg0)
{
#ifdef PLATFORM_LINUX
    signal(SIGABRT, ASL::print_backtrace);
    signal(SIGSEGV, ASL::print_backtrace);
    signal(SIGTRAP, ASL::print_backtrace);
#else
#endif

    Logger::set_process_name(arg0);
}

} // namespace ASL