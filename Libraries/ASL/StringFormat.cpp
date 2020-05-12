#include "StringFormat.h"

#include "String.h"
#include "StringUtils.h"

namespace ASL {

String extract_field_format(const StringView& fmt, Vector<FieldFormat>& field_formats)
{
    // Just in case
    field_formats.clear();

    TriState is_automatic_field_index = TriState::Unknown;

    int argument_index = 0;
    size_t offset = 0;
    for (;;) {
        auto pos = fmt.index_of('{', offset);
        if (!pos.has_value())
            break;

        size_t start = pos.value();
        if (start < fmt.length() - 1 && fmt[start + 1] == '{') {
            offset = start + 2;
            continue;
        }

        pos = fmt.index_of('}', start);
        if (!pos.has_value())
            break;

        size_t end = pos.value();

        // slice of the '{' and '}'
        StringView current_format = fmt.substring_view(start + 1, end - start - 1);

        FieldFormat arg;
        arg.string = current_format;
        arg.field_index = argument_index;

        if (current_format.is_empty()) {
            if (is_automatic_field_index == TriState::False)
                return "<error>Can not switch from manual field index to automatic field index</error>";
            else if (is_automatic_field_index == TriState::Unknown)
                is_automatic_field_index = TriState::True;

            field_formats.emplace(true, fmt.substring_view(offset, start - offset));
            field_formats.append(arg);
            offset = end + 1;
            ++argument_index;

            continue;
        }

        bool ok;
        auto colon_pos = current_format.index_of(':');

        // Parse field index
        if (!colon_pos.has_value() || colon_pos.value() > 0) {
            if (is_automatic_field_index == TriState::True) {
                return "<error>Can not switch from automatic field index to manual field index</error>";
            }

            if (is_automatic_field_index == TriState::Unknown)
                is_automatic_field_index = TriState::False;

            size_t number_length = colon_pos.value_or(current_format.length());
            arg.field_index = string_to_uint(current_format.substring_view(0, number_length), &ok);
            if (!ok)
                return "<error>Can not parse field index</error>";

        } else if (is_automatic_field_index == TriState::Unknown) {
            is_automatic_field_index = TriState::True;
        }

        auto dot_pos = current_format.index_of('.');

        // Parse padding
        if (colon_pos.has_value()) {
            size_t padding_start = colon_pos.value() + 1;
            size_t padding_end = dot_pos.value_or(current_format.length());
            if (padding_start < padding_end) {
                arg.padding = string_to_int(current_format.substring_view(padding_start, padding_end - padding_start), &ok);
                if (!ok)
                    return "<error> Can not parse field padding</error>";
            }
        }

        // Parse precision
        if (colon_pos.has_value() && dot_pos.has_value()) {
            arg.precision = string_to_uint(current_format.substring_view(dot_pos.value() + 1), &ok);
            if (!ok)
                return "<error> Can not parse field precision</error>";
        }

        field_formats.emplace(true, fmt.substring_view(offset, start - offset));
        field_formats.append(arg);
        offset = end + 1;
        ++argument_index;
    }

    if (offset < fmt.length())
        field_formats.emplace(true, fmt.substring_view(offset));

    return String::empty();
}

} // namespace ASL