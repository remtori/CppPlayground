#pragma once

#include "Demangle.h"
#include "Forward.h"
#include "StdLibExtras.h"
#include "String.h"
#include "StringBuilder.h"

namespace ASL {

template<class T>
constexpr auto call_to_string_if_present(const T& obj, String& result) -> decltype(obj.to_string(), TrueType {})
{
    result = obj.to_string();
    return {};
}

constexpr auto call_to_string_if_present(...) -> FalseType { return {}; }

template<typename T>
String try_to_string(const T& any)
{
    if constexpr (CanConstructString<T>::value)
        return any;

    String result;
    if constexpr (IsPointer<T>::value)
        call_to_string_if_present(*any, result);
    else
        call_to_string_if_present(any, result);

    if (!result.is_empty())
        return result;

    StringBuilder builder(96);
    builder.append("<error>class ");
    builder.append(demangle(typeid(T).name()));
    builder.append(" is missing ::to_string() method</error>");
    return builder.to_string();
}

} // namespace ASL