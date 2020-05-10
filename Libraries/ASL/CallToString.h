#pragma once

#include "Forward.h"
#include "StdLibExtras.h"

namespace ASL {

template<class T>
constexpr auto call_to_string_if_present(const T* obj, String& result) -> decltype(obj->to_string(), TrueType {})
{
    result = obj->to_string();
    return {};
}

constexpr auto call_to_string_if_present(...) -> FalseType
{
    return {};
}

} // namespace ASL