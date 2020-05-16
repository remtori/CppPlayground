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

#if defined(__GNUC__) && defined(__BYTE_ORDER__)
#    define __BYTE_ORDER __BYTE_ORDER__
#else
#    include <bits/endian.h>
#endif

#if defined(PLATFORM_LINUX)

#    include <endian.h>

#elif defined(PLATFORM_WINDOWS)

#    include <windows.h>

#    if defined(__GNUC__) || defined(__clang__)
static inline uint16_t __bswap16(uint16_t x) { return __builtin_bswap16(x); }
static inline uint32_t __bswap32(uint32_t x) { return __builtin_bswap32(x); }
static inline uint64_t __bswap64(uint64_t x) { return __builtin_bswap64(x); }
#    elif defined(_MSC_VER)
#        include <stdlib.h>
static inline uint16_t __bswap16(uint16_t x) { return _byteswap_ushort(x); }
static inline uint32_t __bswap32(uint32_t x) { return _byteswap_ulong(x); }
static inline uint64_t __bswap64(uint64_t x) { return _byteswap_uint64(x); }
#    endif

#    define LITTLE_ENDIAN __LITTLE_ENDIAN
#    define BIG_ENDIAN __BIG_ENDIAN
#    define PDP_ENDIAN __PDP_ENDIAN
#    define BYTE_ORDER __BYTE_ORDER

#    if BYTE_ORDER == LITTLE_ENDIAN
#        define htole16(x) ((uint16_t)(x))
#        define le16toh(x) ((uint16_t)(x))
#        define letoh16(x) ((uint16_t)(x))
#        define htole32(x) ((uint32_t)(x))
#        define le32toh(x) ((uint32_t)(x))
#        define letoh32(x) ((uint32_t)(x))
#        define htole64(x) ((uint64_t)(x))
#        define le64toh(x) ((uint64_t)(x))
#        define letoh64(x) ((uint64_t)(x))
#        define htobe16(x) (__bswap16(x))
#        define be16toh(x) (__bswap16(x))
#        define betoh16(x) (__bswap16(x))
#        define htobe32(x) (__bswap32(x))
#        define be32toh(x) (__bswap32(x))
#        define betoh32(x) (__bswap32(x))
#        define htobe64(x) (__bswap64(x))
#        define be64toh(x) (__bswap64(x))
#        define betoh64(x) (__bswap64(x))
#    else
#        define htole16(x) (__bswap16(x))
#        define le16toh(x) (__bswap16(x))
#        define letoh16(x) (__bswap16(x))
#        define htole32(x) (__bswap32(x))
#        define le32toh(x) (__bswap32(x))
#        define letoh32(x) (__bswap32(x))
#        define htole64(x) (__bswap64(x))
#        define le64toh(x) (__bswap64(x))
#        define letoh64(x) (__bswap64(x))
#        define htobe16(x) ((uint16_t)(x))
#        define be16toh(x) ((uint16_t)(x))
#        define betoh16(x) ((uint16_t)(x))
#        define htobe32(x) ((uint32_t)(x))
#        define be32toh(x) ((uint32_t)(x))
#        define betoh32(x) ((uint32_t)(x))
#        define htobe64(x) ((uint64_t)(x))
#        define be64toh(x) ((uint64_t)(x))
#        define betoh64(x) ((uint64_t)(x))
#    endif

#endif
