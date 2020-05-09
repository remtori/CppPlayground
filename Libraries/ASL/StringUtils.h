#pragma once

#include "StringView.h"
#include "Types.h"

namespace ASL {

u32 string_to_uint(const StringView&, bool* ok = nullptr);
i32 string_to_int(const StringView&, bool* ok = nullptr);
double string_to_double(const StringView&, bool* ok = nullptr);
double string_e_to_double(const StringView&, bool* ok = nullptr);

} // namespace ASL
