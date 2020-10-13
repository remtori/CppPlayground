#pragma once

#include <ASL/Attributes.h>
#include <ASL/Types.h>
#include <stdlib.h>

namespace ASL {

struct Allocator {
    NO_DISCARD constexpr void* allocate(size_t size)
    {
        return malloc(size);
    }

    constexpr void deallocate(void* ptr)
    {
        free(ptr);
    }
};

} // namespace ASL