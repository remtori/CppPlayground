#pragma once

#include <ASL/Backtrace.h>
#include <ASL/Platform.h>

#define ASL_LOGGER_IMPL
#include <ASL/Logger.h>

#include <signal.h>

namespace ASL {

void initialize(const char* arg0)
{
    signal(SIGABRT, ASL::print_backtrace);
    signal(SIGFPE, ASL::print_backtrace);
    signal(SIGILL, ASL::print_backtrace);
    signal(SIGINT, ASL::print_backtrace);
    signal(SIGSEGV, ASL::print_backtrace);

    Logger::set_process_name(arg0);
}

} // namespace ASL