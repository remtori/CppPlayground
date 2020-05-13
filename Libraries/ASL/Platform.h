#pragma once

#ifdef _WIN32
#    define PLATFORM_WINDOWS
#    ifdef _WIN64
#        define PLATFORM_WINDOWS_x64
#    else
#        define PLATFORM_WINDOWS_x32
#    endif
#elif defined(__APPLE__) || defined(__MACH__)
#    include <TargetConditionals.h>
#    if TARGET_IPHONE_SIMULATOR == 1
#        define PLATFORM_IOS
#    elif TARGET_OS_IPHONE == 1
#        define PLATFORM_IOS
#    elif TARGET_OS_MAC == 1
#        define PLATFORM_MACOS
#    else
#        error "Unknown Apple platform!"
#    endif
#elif defined(__ANDROID__)
#    define PLATFORM_ANDROID
#elif defined(__linux__)
#    define PLATFORM_LINUX
#else
#    error "Unknown platform!"
#endif
