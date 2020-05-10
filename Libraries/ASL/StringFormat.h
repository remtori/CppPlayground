// BROKEN - DO NOT USE
#pragma once

#include "CallToString.h"
#include "StdLibExtras.h"
#include "String.h"
#include "StringBuilder.h"
#include "StringView.h"
#include "Vector.h"
#include <stdio.h>

namespace ASL {

template<typename T>
constexpr StringView any_to_string_view(const T& value, const char* fmt = "%f")
{
    if constexpr (IsInteger<T>::value)
        return String::number(value);

    if constexpr (IsFloat<T>::value) {
        char buf[32];
        sprintf(buf, fmt, value);
        return StringView(buf);
    }

    String result = "<error-type-unknown>";

    if constexpr (IsPointer<T>::value)
        call_to_string_if_present(value, result);
    else
        call_to_string_if_present(&value, result);

    return result;
}

class StringFormat {
public:
    template<class... Args>
    static String format(StringView fmt, const Args&... input_args)
    {
        // StringBuilder builder;
        // Vector<String> arg_format;
        // Vector<StringView> args;

        // auto resolve = [&](auto&& arg) {
        //     builder.append(any_to_string_view(arg));
        // };

        // (resolve(input_args), ...);

        // return builder.to_string();
        return "Unimplemented";
    }
};

} // namespace ASL

using ASL::StringFormat;
