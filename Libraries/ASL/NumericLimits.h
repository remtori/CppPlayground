#pragma once

#include "Types.h"

#ifndef _MSC_VER
#    define SCHAR_MAX __SCHAR_MAX__
#    define SHRT_MAX __SHRT_MAX__
#    define INT_MAX __INT_MAX__
#    define LONG_MAX __LONG_MAX__
#    define LLONG_MAX __LONG_LONG_MAX__
#    define FLT_MIN __FLT_MIN__
#    define FLT_MAX __FLT_MAX__
#    define DBL_MIN __DBL_MIN__
#    define DBL_MAX __DBL_MAX__
#    define LDBL_MIN __LDBL_MIN__
#    define LDBL_MAX __LDBL_MAX__
#endif

namespace ASL {

template<typename T>
struct NumericLimits {
};

template<>
struct NumericLimits<bool> {
    static constexpr char min() { return false; }
    static constexpr char max() { return true; }
    static constexpr bool is_signed() { return false; }
};

template<>
struct NumericLimits<char> {
    static constexpr char min() { return -SCHAR_MAX - 1; }
    static constexpr char max() { return SCHAR_MAX; }
    static constexpr bool is_signed() { return true; }
};

template<>
struct NumericLimits<short> {
    static constexpr short min() { return -SHRT_MAX - 1; }
    static constexpr short max() { return SHRT_MAX; }
    static constexpr bool is_signed() { return true; }
};

template<>
struct NumericLimits<int> {
    static constexpr int min() { return -INT_MAX - 1; }
    static constexpr int max() { return INT_MAX; }
    static constexpr bool is_signed() { return true; }
};

template<>
struct NumericLimits<long> {
    static constexpr long min() { return -LONG_MAX - 1; }
    static constexpr long max() { return LONG_MAX; }
    static constexpr bool is_signed() { return true; }
};

template<>
struct NumericLimits<long long> {
    static constexpr long long min() { return -LLONG_MAX - 1; }
    static constexpr long long max() { return LLONG_MAX; }
    static constexpr bool is_signed() { return true; }
};

template<>
struct NumericLimits<unsigned char> {
    static constexpr unsigned char min() { return 0; }
    static constexpr unsigned char max() { return SCHAR_MAX * 2u + 1; }
    static constexpr bool is_signed() { return false; }
};

template<>
struct NumericLimits<unsigned short> {
    static constexpr unsigned short min() { return 0; }
    static constexpr unsigned short max() { return SHRT_MAX * 2u + 1; }
    static constexpr bool is_signed() { return false; }
};

template<>
struct NumericLimits<unsigned> {
    static constexpr unsigned min() { return 0; }
    static constexpr unsigned max() { return INT_MAX * 2u + 1; }
    static constexpr bool is_signed() { return false; }
};

template<>
struct NumericLimits<unsigned long> {
    static constexpr unsigned long min() { return 0; }
    static constexpr unsigned long max() { return LONG_MAX * 2ul + 1; }
    static constexpr bool is_signed() { return false; }
};

template<>
struct NumericLimits<unsigned long long> {
    static constexpr unsigned long long min() { return 0; }
    static constexpr unsigned long long max() { return LLONG_MAX * 2ull + 1; }
    static constexpr bool is_signed() { return false; }
};

template<>
struct NumericLimits<float> {
    static constexpr float min() { return FLT_MIN; }
    static constexpr float max() { return FLT_MAX; }
    static constexpr bool is_signed() { return true; }
};

template<>
struct NumericLimits<double> {
    static constexpr double min() { return DBL_MIN; }
    static constexpr double max() { return DBL_MAX; }
    static constexpr bool is_signed() { return true; }
};

template<>
struct NumericLimits<long double> {
    static constexpr long double min() { return LDBL_MIN; }
    static constexpr long double max() { return LDBL_MAX; }
    static constexpr bool is_signed() { return true; }
};

} // namespace ASL

using ASL::NumericLimits;
