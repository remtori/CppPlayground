#include "StringFormat.h"

#include "String.h"
#include "StringBuilder.h"
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
            field_formats.emplace(true, fmt.substring_view(offset, start - offset + 2));
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
        auto parts = current_format.split_view(':', true);
        StringView field_index = parts[0];

        // Parse field index
        if (!field_index.is_empty()) {
            if (is_automatic_field_index == TriState::True)
                return "<error>Can not switch from automatic field index to manual field index</error>";
            else if (is_automatic_field_index == TriState::Unknown)
                is_automatic_field_index = TriState::False;

            arg.field_index = string_to_uint(field_index, &ok);
            if (!ok)
                return "<error>Can not parse field index</error>";

        } else if (is_automatic_field_index == TriState::Unknown) {
            is_automatic_field_index = TriState::True;
        }

        if (parts.size() == 2) {
            parts = parts[1].split_view('.', true);
            StringView field_padding = parts[0];
            StringView field_precision;
            if (parts.size() == 2)
                field_precision = parts[1];

            // Parse padding
            if (!field_padding.is_empty()) {
                arg.padding = string_to_int(field_padding, &ok);
                if (!ok)
                    return "<error> Can not parse field padding</error>";
            }

            // Parse precision
            if (!field_precision.is_empty()) {
                arg.precision = string_to_uint(field_precision, &ok);
                if (!ok)
                    return "<error> Can not parse field precision</error>";
            }
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

String build_result(Vector<FieldFormat> field_formats, HashMap<size_t, String> field_values)
{

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

} // namespace ASL