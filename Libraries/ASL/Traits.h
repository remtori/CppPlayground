#pragma once

#include "HashFunctions.h"
#include "Types.h"

namespace ASL {

template<typename T>
struct GenericTraits {
    using PeekType = T;
    static constexpr bool is_trivial() { return false; }
    static bool equals(const T& a, const T& b) { return a == b; }
};

template<typename T>
struct Traits : public GenericTraits<T> {
};

template<>
struct Traits<int> : public GenericTraits<int> {
    static constexpr bool is_trivial() { return true; }
    static unsigned hash(int i) { return int_hash(i); }
};

template<>
struct Traits<unsigned> : public GenericTraits<unsigned> {
    static constexpr bool is_trivial() { return true; }
    static unsigned hash(unsigned u) { return int_hash(u); }
};

template<>
struct Traits<u16> : public GenericTraits<u16> {
    static constexpr bool is_trivial() { return true; }
    static unsigned hash(u16 u) { return int_hash(u); }
};

template<>
struct Traits<u64> : public GenericTraits<u64> {
    static constexpr bool is_trivial() { return true; }
    static unsigned hash(u64 u) { return u64_hash(u); }
};

template<>
struct Traits<char> : public GenericTraits<char> {
    static constexpr bool is_trivial() { return true; }
    static unsigned hash(char c) { return int_hash(c); }
};

template<typename T>
struct Traits<T*> : public GenericTraits<T*> {
    static unsigned hash(const T* p)
    {
        return int_hash((unsigned)(__PTRDIFF_TYPE__)p);
    }
    static constexpr bool is_trivial() { return true; }
    static bool equals(const T* a, const T* b) { return a == b; }
};

} // namespace ASL

using ASL::GenericTraits;
using ASL::Traits;
