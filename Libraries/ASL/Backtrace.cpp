#include "Backtrace.h"

#include "Demangle.h"
#include "LogStream.h"

#ifdef PLATFORM_LINUX
#    include <cxxabi.h>
#    include <execinfo.h>
#elif defined(PLATFORM_WINDOWS)
#endif

namespace ASL {

constexpr const size_t backtrace_size = 21;

void print_backtrace(int signal)
{
#ifdef PLATFORM_LINUX
    void* array[backtrace_size];
    size_t size;
    size = backtrace(array, backtrace_size);
    char** messages = backtrace_symbols(array, backtrace_size);

    {
        dbg() << '(' << strsignal(signal) << ") Signal " << signal;
    }

    // Skip the first stack frame (this function)
    for (size_t i = 1; i < size && messages != nullptr; ++i) {
        char* mangled_name = 0;
        char* offset_begin = 0;
        char* offset_end = 0;

        for (char* p = messages[i]; *p; ++p) {
            if (*p == '(')
                mangled_name = p;
            else if (*p == '+')
                offset_begin = p;
            else if (*p == ')') {
                offset_end = p;
                break;
            }
        }

        if (mangled_name && offset_begin && offset_end && mangled_name < offset_begin) {
            *mangled_name++ = '\0';
            *offset_begin++ = '\0';
            *offset_end++ = '\0';

            dbg() << "[callstack]: (" << i << ") " << messages[i] << ": " << demangle(mangled_name);
        } else {
            dbg() << "[callstack]: (" << i << ") " << messages[i];
        }
#elif defined(PLATFORM_WINDOWS)

#endif
    }
} // namespace ASL

} // namespace ASL
