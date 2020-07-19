#pragma once

#ifndef DISABLE_ASSERT
#    include <assert.h>
#    define ASSERT assert
#else
#    define ASSERT(x)
#endif

#define ASSERT_NOT_REACHED() ASSERT(false)