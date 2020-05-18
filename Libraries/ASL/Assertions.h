#pragma once

#include "assert.h"

#ifdef _MSC_VER
#    define DEBUG_BREAK __debugbreak
#else
#    define DEBUG_BREAK()
#endif

#ifndef DISABLE_ASSERT

#    define ASSERT(x)      \
        if (!(x)) {        \
            assert((x));   \
            DEBUG_BREAK(); \
        }

#else
#    define ASSERT(x)
#endif

#define ASSERT_NOT_REACHED() ASSERT(false)
