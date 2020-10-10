#pragma once

#include <ASL/Types.h>

namespace ASL {

template<typename T>
struct GenericTraits {
    using PeekType = T;
    static constexpr bool is_trivial() { return false; }
    static int compare(const T& a, const T& b)
    {
        if (a == b)
            return 0;
        return a > b ? 1 : -1;
    }

    static bool equals(const T& a, const T& b) { return compare(a, b) == 0; }
};

template<typename T>
struct Traits : public GenericTraits<T> {
};

template<>
struct Traits<u8> : public GenericTraits<u8> {
    static constexpr bool is_trivial() { return true; }
};

template<>
struct Traits<i8> : public GenericTraits<i8> {
    static constexpr bool is_trivial() { return true; }
};

template<>
struct Traits<u16> : public GenericTraits<u16> {
    static constexpr bool is_trivial() { return true; }
};

template<>
struct Traits<i16> : public GenericTraits<i16> {
    static constexpr bool is_trivial() { return true; }
};

template<>
struct Traits<u32> : public GenericTraits<u32> {
    static constexpr bool is_trivial() { return true; }
};

template<>
struct Traits<i32> : public GenericTraits<i32> {
    static constexpr bool is_trivial() { return true; }
};

template<>
struct Traits<u64> : public GenericTraits<u64> {
    static constexpr bool is_trivial() { return true; }
};

template<>
struct Traits<i64> : public GenericTraits<i64> {
    static constexpr bool is_trivial() { return true; }
};

template<>
struct Traits<float> : public GenericTraits<float> {
    static constexpr bool is_trivial() { return true; }
};

template<>
struct Traits<double> : public GenericTraits<double> {
    static constexpr bool is_trivial() { return true; }
};

template<typename T>
struct Traits<T*> : public GenericTraits<T*> {
    static constexpr bool is_trivial() { return true; }
    static bool equals(const T* a, const T* b) { return a == b; }
    static int compare(const T* a, const T* b)
    {
        if (a == b)
            return 0;
        return (__PTRDIFF_TYPE__)a > (__PTRDIFF_TYPE__)b ? 1 : -1;
    }
};

} // namespace ASL

using ASL::GenericTraits;
using ASL::Traits;
