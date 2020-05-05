#pragma once

namespace ASL {

template<typename T>
inline constexpr T min(const T& a, const T& b)
{
    return a < b ? a : b;
}

template<typename T>
inline constexpr T max(const T& a, const T& b)
{
    return a > b ? a : b;
}

template<typename T>
inline T&& move(T& a)
{
    return static_cast<T&&>(a);
}

template<typename T, typename U>
inline void swap(T& a, U& b)
{
    U tmp = move((U&)a);
    a = (T &&) move(b);
    b = move(tmp);
}

template<class T, T v>
struct IntegralConstant {
    static constexpr T value = v;
    using ValueType = T;
    using Type = IntegralConstant;
    constexpr operator ValueType() const { return value; }
    constexpr ValueType operator()() const { return value; }
};

using FalseType = IntegralConstant<bool, false>;
using TrueType = IntegralConstant<bool, true>;

template<class T>
struct IsLvalueReference : FalseType {
};

template<class T>
struct IsLvalueReference<T&> : TrueType {
};

template<typename T>
struct RemoveReference {
    using Type = T;
};
template<class T>
struct RemoveReference<T&> {
    using Type = T;
};
template<class T>
struct RemoveReference<T&&> {
    using Type = T;
};

template<class T>
inline constexpr T&& forward(typename RemoveReference<T>::Type& param)
{
    return static_cast<T&&>(param);
}

template<class T>
inline constexpr T&& forward(typename RemoveReference<T>::Type&& param) noexcept
{
    static_assert(!IsLvalueReference<T>::value, "Can't forward an rvalue as an lvalue.");
    return static_cast<T&&>(param);
}

} // namespace ASL

using ASL::forward;
using ASL::max;
using ASL::min;
using ASL::move;
using ASL::swap;
