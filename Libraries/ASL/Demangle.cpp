#include <ASL/Demangle.h>

#include <ASL/String.h>

#ifdef _MSC_VER
#    include <DbgHelp.h>
#    include <Windows.h>
#else
#    include <cxxabi.h>
#endif

namespace ASL {

String demangle(const StringView& name)
{
#ifdef _MSC_VER
    static char demangled_name[1024];
    int size = UnDecorateSymbolName(String(name).characters(), &demangled_name[0], 1024, UNDNAME_COMPLETE);
    if (size > 0) {
        demangled_name[size] = '\0';
        return demangled_name;
    }

    return name;
#else
    int status = 0;
    char* demangled_name = abi::__cxa_demangle(String(name).characters(), nullptr, nullptr, &status);
    String string = status == 0 ? String(demangled_name) : String(name);
    if (status == 0)
        std::free(demangled_name);

    return string;
#endif
}

} // namespace ASL
