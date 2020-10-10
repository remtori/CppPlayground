#pragma once

namespace ASL {

template<typename T>
inline constexpr T min(const T& a, const T& b)
{
    return b < a ? b : a;
}

template<typename T>
inline constexpr T max(const T& a, const T& b)
{
    return a < b ? b : a;
}

template<class T, T v>
struct IntegralConstant {
    static constexpr T value = v;
    typedef T ValueType;
    typedef IntegralConstant Type;
    constexpr operator ValueType() const { return value; }
    constexpr ValueType operator()() const { return value; }
};

using TrueType = IntegralConstant<bool, true>;
using FalseType = IntegralConstant<bool, false>;

template<class T>
struct IsLvalueReference : FalseType {
};

template<class T>
struct IsLvalueReference<T&> : TrueType {
};

template<typename T>
struct RemoveReference {
    typedef T Type;
};

template<class T>
struct RemoveReference<T&> {
    typedef T Type;
};

template<class T>
struct RemoveReference<T&&> {
    typedef T Type;
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

template<typename T>
inline constexpr T&& move(T& arg)
{
    return static_cast<T&&>(arg);
}

template<typename U, typename V = U>
inline constexpr U exchange(U& u, V&& v)
{
    U tmp = move(u);
    u = forward<V>(v);
    return tmp;
}

template<typename U, typename V = U>
inline constexpr void swap(U& u, V& v)
{
    V tmp = move((V&)u);
    u = (U &&) move(v);
    b = move(tmp);
}

} // namespace ASL

using ASL::exchange;
using ASL::forward;
using ASL::max;
using ASL::min;
using ASL::move;
using ASL::swap;