#pragma once

#include <ASL/Backtrace.h>
#include <ASL/Logger.h>

namespace ASL {

void initialize(const char* arg0)
{
    HANDLE_CRASH();
    Logger::set_process_name(arg0);
}

} // namespace ASL