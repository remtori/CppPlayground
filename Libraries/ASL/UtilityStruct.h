#pragma once

#include <ASL/Types.h>

namespace ASL {

template<typename T>
struct BasePosition {
    T x = 0;
    T y = 0;
};

template<typename T>
struct BaseSize {
    T width = 0;
    T height = 0;
};

template<typename T>
struct BaseRect {
    T x = 0;
    T y = 0;
    T width = 0;
    T height = 0;
};

using Position = BasePosition<i32>;
using FPosition = BasePosition<float>;
using Size = BaseSize<i32>;
using FSize = BaseSize<float>;
using Rect = BaseRect<i32>;
using FRect = BaseRect<float>;

} // namespace ASL

using ASL::FPosition;
using ASL::FRect;
using ASL::FSize;
using ASL::Position;
using ASL::Rect;
using ASL::Size;
