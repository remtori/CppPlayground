#pragma once

#include "assert.h"

#ifdef _MSC_VER
#define ASSERT(x) if(!(x)) { __debugbreak(); assert(false); }
#else
#define ASSERT assert
#endif

#define ASSERT_NOT_REACHED() ASSERT(false)
