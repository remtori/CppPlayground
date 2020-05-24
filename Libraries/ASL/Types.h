#pragma once

#include <cstdint>
#include <stddef.h>

using i8 = int8_t;
using u8 = uint8_t;
using i16 = int16_t;
using u16 = uint16_t;
using i32 = int32_t;
using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

static_assert(sizeof(i8) == 1);
static_assert(sizeof(u8) == 1);
static_assert(sizeof(i16) == 2);
static_assert(sizeof(u16) == 2);
static_assert(sizeof(i32) == 4);
static_assert(sizeof(u32) == 4);
static_assert(sizeof(i64) == 8);
static_assert(sizeof(u64) == 8);

using FlatPtr = intptr_t;

constexpr unsigned KB = 1024;
constexpr unsigned MB = KB * KB;
constexpr unsigned GB = KB * KB * KB;

namespace std {
using nullptr_t = decltype(nullptr);
}

enum class TriState : u8 {
    False = 0,
    True = 1,
    Unknown = 2,
};
