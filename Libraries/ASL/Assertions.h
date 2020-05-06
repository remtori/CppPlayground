#pragma once


#ifdef _MSC_VER
#define ASSERT(x) if(!(x)) { __debugbreak(); assert(false); }
#else
#include "assert.h"
#define ASSERT assert
#endif

#define ASSERT_NOT_REACHED() ASSERT(false)
