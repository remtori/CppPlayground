#include "Demangle.h"

#include "String.h"
#include <cxxabi.h>

namespace ASL {

String demangle(const StringView& name)
{
    int status = 0;
    char* demangled_name = abi::__cxa_demangle(String(name).characters(), nullptr, nullptr, &status);
    String string = String(status == 0 ? demangled_name : name);
    if (status == 0)
        std::free(demangled_name);

    return string;
}

} // namespace ASL
