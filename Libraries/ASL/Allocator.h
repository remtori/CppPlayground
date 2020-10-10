#pragma once

#include <ASL/Attributes.h>
#include <ASL/Types.h>
#include <memory>

namespace ASL {

template<typename T>
struct Allocator {
    using ValueType = T;
    using PointerType = T*;

    NO_DISCARD constexpr PointerType allocate(size_t num)
    {
        return static_cast<PointerType>(malloc(sizeof(ValueType) * num));
    }

    constexpr void deallocate(PointerType ptr)
    {
        free(ptr);
    }
};

} // namespace ASL