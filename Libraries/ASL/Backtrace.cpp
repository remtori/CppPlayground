#include <ASL/Backtrace.h>

#include <ASL/Demangle.h>
#include <ASL/Logger.h>
#include <ASL/Platform.h>
#include <string.h>
#include <string>

#include <cxxabi.h>

#if defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS)
#    include <execinfo.h>
#elif defined(PLATFORM_WINDOWS)

#endif

namespace ASL {

constexpr const size_t backtrace_size = 21;

void print_backtrace(int signal)
{

#if defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS)

    void* array[backtrace_size];
    size_t size;
    size = backtrace(array, backtrace_size);
    char** messages = backtrace_symbols(array, backtrace_size);

    LOG_CRITICAL("({}) Signal {}", strsignal(signal), signal);

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

            LOG_WARN("[callstack]: ({:2}) {}", i, demangle(mangled_name));
        } else {
            LOG_WARN("[callstack]: ({:2}) {}", i, messages[i]);
        }
    }
#elif defined(PLATFORM_WINDOWS)
    LOG_CRITICAL("Crashed! Signal {}", signal);
#endif

    exit(1);
}

} // namespace ASL