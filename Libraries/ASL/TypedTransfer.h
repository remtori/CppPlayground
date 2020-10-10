#pragma once

#include <ASL/Traits.h>
#include <memory>

namespace ASL {

// There is a chance that we will overwrite the source
// if we just iterating one way
template<typename T>
struct TypedTransfer {
    static size_t move(T* destination, T* source, size_t count)
    {
        if constexpr (Traits<T>::is_trivial()) {
            memmove(destination, source, count * sizeof(T));
            return count;
        }

        for (size_t i = 0; i < count; i++) {
            if (destination <= source)
                new (&destination[i]) T(move(source[i]));
            else
                new (&destination[count - i - 1]) T(move(source[count - i - 1]));
        }
    }

    static size_t copy(T* destination, T* source, size_t count)
    {
        if constexpr (Traits<T>::is_trivial()) {
            memmove(destination, source, count * sizeof(T));
            return count;
        }

        for (size_t i = 0; i < count; i++) {
            if (destination <= source)
                new (&destination[i]) T(source[i]);
            else
                new (&destination[count - i - 1]) T(source[count - i - 1]);
        }
    }
};

} // namespace ASL