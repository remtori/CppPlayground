#pragma once

#include <ASL/Types.h>

namespace ASL {

inline unsigned int_hash(u32 key)
{
    key += ~(key << 15);
    key ^= (key >> 10);
    key += (key << 3);
    key ^= (key >> 6);
    key += ~(key << 11);
    key ^= (key >> 16);
    return key;
}

inline unsigned pair_int_hash(u32 key1, u32 key2)
{
    return int_hash((int_hash(key1) * 209) ^ (int_hash(key2 * 413)));
}

inline unsigned u64_hash(u64 key)
{
    u32 first = key & 0xFFFFFFFF;
    u32 last = key >> 32;
    return pair_int_hash(first, last);
}

inline unsigned ptr_hash(FlatPtr ptr)
{
    if constexpr (sizeof(ptr) == 8)
        return u64_hash((u64)ptr);
    else
        return int_hash((u32)ptr);
}

inline unsigned ptr_hash(const void* ptr)
{
    return ptr_hash((FlatPtr)(ptr));
}

} // namespace ASL
