#pragma once

#include <ASL/Forward.h>
#include <ASL/Types.h>
#include <memory>

namespace Reme {

template<typename T>
using OwnPtr = std::unique_ptr<T>;

template<typename T, typename... Args>
constexpr OwnPtr<T> create_own(Args&&... args)
{
    return std::make_unique<T>(std::forward<Args>(args)...);
}

} // namespace Reme
