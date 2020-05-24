#pragma once

#include <ASL/StringView.h>
#include <ASL/Types.h>

namespace ASL {

u32 LIB_API string_to_uint(const StringView&, bool* ok = nullptr);
i32 LIB_API string_to_int(const StringView&, bool* ok = nullptr);
double LIB_API string_to_double(const StringView&, bool* ok = nullptr);
double LIB_API string_e_to_double(const StringView&, bool* ok = nullptr);

} // namespace ASL
