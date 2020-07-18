#pragma once

#include <ASL/Platform.h>

#ifdef PLATFORM_WINDOWS
#    ifdef COMPILING_DLL
#        define LIB_API __declspec(dllexport)
#    else
#        define LIB_API __declspec(dllimport)
#    endif
#else
#    define LIB_API
#endif
