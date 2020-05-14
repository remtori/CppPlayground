#pragma once

#include "Forward.h"
#include "HashMap.h"
#include "StdLibExtras.h"
#include "String.h"
#include "StringBuilder.h"
#include "StringView.h"
#include "TryToString.h"
#include "Vector.h"
#include <stdio.h>

namespace ASL {

struct FieldFormat {
    bool is_raw = false;
    int padding = 0;
    int precision = 4;
    size_t field_index = -1;
    StringView string;

    FieldFormat() {}
    ~FieldFormat() {}

    FieldFormat(bool raw, const StringView& str)
        : is_raw(raw)
        , string(str)
    {
    }
};

template<typename T>
static constexpr String format_argument(const T& value, FieldFormat format);

LIB_API String extract_field_format(const StringView&, Vector<FieldFormat>&);

template<class... Args>
String format(const StringView& fmt, const Args&... input_args)
{
    // TODO: Maybe we can also format value to hex or binary format
    Vector<FieldFormat> field_formats;

    String error = extract_field_format(fmt, field_formats);
    if (!error.is_empty())
        return error;

    HashMap<size_t, String> field_values;

    size_t input_arg_count = 0;
    auto index_finder = [&input_arg_count](const FieldFormat& f) {
        return f.field_index == input_arg_count;
    };

    auto resolve = [&](auto&& arg) {
        auto it = field_formats.find(index_finder);

        while (it != field_formats.end()) {
            field_values.set(it.index(), format_argument(arg, *it));
            it = field_formats.find(index_finder, it.index() + 1);
        }

        ++input_arg_count;
    };

    (resolve(input_args), ...);

    StringBuilder builder;

    for (size_t i = 0; i < field_formats.size(); ++i) {
        auto field = field_formats[i];
        if (!field.is_raw) {
            String value = field_values.get_or(i, "Invalid index");

            int space_count = abs(field.padding) - value.length();
            if (space_count > 0 && field.padding > 0)
                builder.append_repeated(' ', space_count);

            builder.append(value);

            if (space_count > 0 && field.padding < 0)
                builder.append_repeated(' ', space_count);

        } else {
            builder.append(field.string);
        }
    }

    return builder.to_string();
}

template<typename T>
static constexpr String format_argument(const T& value, FieldFormat format)
{
    if constexpr (IsInteger<T>::value)
        return String::number(value);

    if constexpr (IsSame<T, float>::value) {
        char buf[32];
        int size = sprintf(buf, "%.*f", format.precision, value);
        return String(buf, size);
    }

    if constexpr (IsSame<T, double>::value) {
        char buf[32];
        int size = sprintf(buf, "%.*lf", format.precision, value);
        return String(buf, size);
    }

    return try_to_string(value);
}

} // namespace ASL

using ASL::format;
