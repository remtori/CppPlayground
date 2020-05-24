#include <ASL/StringUtils.h>

namespace ASL {

inline void set(bool* ok, bool val)
{
    if (ok)
        *ok = val;
}

u32 string_to_uint(const StringView& str, bool* ok)
{
    size_t i = 0;
    if (str[i] == '-') {
        set(ok, false);
        return 0;
    }

    if (str[i] == '+') {
        ++i;
    }

    u32 value = 0;
    for (; i < str.length(); ++i) {
        if ('0' > str[i] || str[i] > '9') {
            set(ok, false);
            return 0;
        }

        value = value * 10 + (str[i] - '0');
    }

    set(ok, true);
    return value;
}

i32 string_to_int(const StringView& str, bool* ok)
{
    size_t i = 0;
    int sign = 1;

    if (str[i] == '-' || str[i] == '+') {
        if (str[i] == '-')
            sign = -1;

        ++i;
    }

    i32 value = 0;
    for (; i < str.length(); ++i) {
        if ('0' > str[i] || str[i] > '9') {
            set(ok, false);
            return 0;
        }

        value = value * 10 + (str[i] - '0');
    }

    set(ok, true);
    return value * sign;
}

double string_to_double(const StringView& str, bool* ok)
{
    auto it = str.index_of('.');
    if (!it.has_value()) {
        return string_to_int(str, ok);
    }

    bool alright;
    i32 whole = string_to_int(str.substring_view(0, it.value()), &alright);
    if (!alright) {
        set(ok, alright);
        return 0;
    }

    auto fraction_str = str.substring_view(it.value() + 1, str.length() - it.value() - 1);
    u32 fraction = string_to_uint(fraction_str, &alright);
    if (!alright) {
        set(ok, alright);
        return 0;
    }

    i32 divider = (whole < 0) ? -1 : 1;
    for (size_t i = 0; i < fraction_str.length(); ++i)
        divider *= 10;

    set(ok, true);
    return (double)whole + ((double)fraction / (double)divider);
}

double string_e_to_double(const StringView& str, bool* ok)
{
    auto it = str.index_of('e');
    if (!it.has_value())
        it = str.index_of('E');

    if (!it.has_value()) {
        set(ok, false);
        return 0;
    }

    bool alright;
    double v = string_to_double(str.substring_view(0, it.value()), &alright);
    if (!alright) {
        set(ok, alright);
        return 0;
    }

    i32 power = string_to_int(str.substring_view(it.value() + 1), &alright);
    if (!alright) {
        set(ok, alright);
        return 0;
    }

    while (power > 0) {
        v *= 10;
        --power;
    }

    while (power < 0) {
        v /= 10;
        ++power;
    }

    return v;
}

} // namespace ASL
