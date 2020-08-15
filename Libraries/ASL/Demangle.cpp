#include <ASL/Demangle.h>

#ifdef _MSC_VER
#    include <Windows.h>
#    include <DbgHelp.h>
#else
#    include <cxxabi.h>
#endif

namespace ASL {

std::string demangle(std::string_view name)
{
#ifdef _MSC_VER
    static char demangled_name[1024];
    std::string str(name);
    int size = UnDecorateSymbolName(str.data(), &demangled_name[0], 1024, UNDNAME_COMPLETE);
    if (size > 0) {
        demangled_name[size] = '\0';
        return demangled_name;
    }

    return str;
#else
    int status = 0;
    char* demangled_name = abi::__cxa_demangle(std::string(name).data(), nullptr, nullptr, &status);
    std::string string = status == 0 ? std::string(demangled_name) : std::string(name);
    if (status == 0)
        std::free(demangled_name);

    return string;
#endif
}

} // namespace ASL
