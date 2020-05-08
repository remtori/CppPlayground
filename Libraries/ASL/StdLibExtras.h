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

template<class>
struct IsFunction : FalseType {
};

template<class Ret, class... Args>
struct IsFunction<Ret(Args...)> : TrueType {
};
template<class Ret, class... Args>
struct IsFunction<Ret(Args..., ...)> : TrueType {
};
template<class Ret, class... Args>
struct IsFunction<Ret(Args...) const> : TrueType {
};
template<class Ret, class... Args>
struct IsFunction<Ret(Args..., ...) const> : TrueType {
};
template<class Ret, class... Args>
struct IsFunction<Ret(Args...) volatile> : TrueType {
};
template<class Ret, class... Args>
struct IsFunction<Ret(Args..., ...) volatile> : TrueType {
};
template<class Ret, class... Args>
struct IsFunction<Ret(Args...) const volatile> : TrueType {
};
template<class Ret, class... Args>
struct IsFunction<Ret(Args..., ...) const volatile> : TrueType {
};

template<class Ret, class... Args>
struct IsFunction<Ret(Args...)&> : TrueType {
};
template<class Ret, class... Args>
struct IsFunction<Ret(Args..., ...)&> : TrueType {
};
template<class Ret, class... Args>
struct IsFunction<Ret(Args...) const&> : TrueType {
};
template<class Ret, class... Args>
struct IsFunction<Ret(Args..., ...) const&> : TrueType {
};
template<class Ret, class... Args>
struct IsFunction<Ret(Args...) volatile&> : TrueType {
};
template<class Ret, class... Args>
struct IsFunction<Ret(Args..., ...) volatile&> : TrueType {
};
template<class Ret, class... Args>
struct IsFunction<Ret(Args...) const volatile&> : TrueType {
};
template<class Ret, class... Args>
struct IsFunction<Ret(Args..., ...) const volatile&> : TrueType {
};

template<class Ret, class... Args>
struct IsFunction<Ret(Args...) &&> : TrueType {
};
template<class Ret, class... Args>
struct IsFunction<Ret(Args..., ...) &&> : TrueType {
};
template<class Ret, class... Args>
struct IsFunction<Ret(Args...) const&&> : TrueType {
};
template<class Ret, class... Args>
struct IsFunction<Ret(Args..., ...) const&&> : TrueType {
};
template<class Ret, class... Args>
struct IsFunction<Ret(Args...) volatile&&> : TrueType {
};
template<class Ret, class... Args>
struct IsFunction<Ret(Args..., ...) volatile&&> : TrueType {
};
template<class Ret, class... Args>
struct IsFunction<Ret(Args...) const volatile&&> : TrueType {
};
template<class Ret, class... Args>
struct IsFunction<Ret(Args..., ...) const volatile&&> : TrueType {
};

template<class T>
struct IsRvalueReference : FalseType {
};
template<class T>
struct IsRvalueReference<T&&> : TrueType {
};

template<class T>
struct RemovePointer {
    using Type = T;
};
template<class T>
struct RemovePointer<T*> {
    using Type = T;
};
template<class T>
struct RemovePointer<T* const> {
    using Type = T;
};
template<class T>
struct RemovePointer<T* volatile> {
    using Type = T;
};
template<class T>
struct RemovePointer<T* const volatile> {
    using Type = T;
};

template<class T>
struct RemoveConst {
    using Type = T;
};

template<class T>
struct RemoveConst<const T> {
    using Type = T;
};

template<class T>
struct RemoveVolatile {
    using Type = T;
};

template<class T>
struct RemoveVolatile<volatile T> {
    using Type = T;
};

template<class T>
struct RemoveCV {
    using Type = typename RemoveVolatile<typename RemoveConst<T>::Type>::Type;
};

template<class T>
struct __IsPointerHelper : FalseType {
};

template<class T>
struct __IsPointerHelper<T*> : TrueType {
};

template<class T>
struct IsPointer : __IsPointerHelper<typename RemoveCV<T>::Type> {
};

template<typename T, typename U>
struct IsSame {
    enum {
        value = 0
    };
};

template<typename T>
struct IsSame<T, T> {
    enum {
        value = 1
    };
};

template<bool condition, class TrueType, class FalseType>
struct Conditional {
    using Type = TrueType;
};

template<class TrueType, class FalseType>
struct Conditional<false, TrueType, FalseType> {
    using Type = FalseType;
};

template<bool B, class T = void>
struct EnableIf {
};

template<class T>
struct EnableIf<true, T> {
    using Type = T;
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

template<typename T, typename U = T>
inline constexpr T exchange(T& slot, U&& value)
{
    T old_value = move(slot);
    slot = forward<U>(value);
    return old_value;
}

} // namespace ASL

using ASL::Conditional;
using ASL::exchange;
using ASL::forward;
using ASL::max;
using ASL::min;
using ASL::move;
using ASL::swap;
