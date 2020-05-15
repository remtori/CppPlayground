#pragma once

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

using Position = BasePosition<int>;
using FPosition = BasePosition<float>;
using Size = BaseSize<int>;
using FSize = BaseSize<float>;
using Rect = BaseRect<int>;
using FRect = BaseRect<float>;

} // namespace ASL

using ASL::FPosition;
using ASL::FRect;
using ASL::FSize;
using ASL::Position;
using ASL::Rect;
using ASL::Size;
