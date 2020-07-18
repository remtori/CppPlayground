#pragma once

#include <stdint.h>

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
static_assert(sizeof(float) == 4);
static_assert(sizeof(double) == 8);

constexpr u32 KB = 1024;
constexpr u32 MB = KB * KB;
constexpr u32 GB = KB * KB * KB;

namespace std {
using nullptr_t = decltype(nullptr);
}
