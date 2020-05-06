#pragma once

#include "StdLibExtras.h"
#include <string.h>

namespace ASL {

template<typename T>
class TypedTransfer {
public:
    static void move(T* destination, T* source, size_t count)
    {
        if (!count)
            return;
        if constexpr (Traits<T>::is_trivial()) {
            memmove(destination, source, count * sizeof(T));
            return;
        }
        for (size_t i = 0; i < count; ++i)
            new (&destination[i]) T(ASL::move(source[i]));
    }

    static void copy(T* destination, const T* source, size_t count)
    {
        if (!count)
            return;
        if constexpr (Traits<T>::is_trivial()) {
            memmove(destination, source, count * sizeof(T));
            return;
        }
        for (size_t i = 0; i < count; ++i)
            new (&destination[i]) T(source[i]);
    }

    static bool compare(const T* a, const T* b, size_t count)
    {
        if (!count)
            return true;

        if constexpr (Traits<T>::is_trivial())
            return !memcmp(a, b, count * sizeof(T));

        for (size_t i = 0; i < count; ++i) {
            if (a[i] != b[i])
                return false;
        }
        return true;
    }
};

} // namespace ASL

using ASL::TypedTransfer;
