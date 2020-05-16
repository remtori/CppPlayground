#pragma once

#include <ASL/Types.h>

namespace Codec {

u32 compute_crc_32(const void* data, size_t length);

} // namespace Codec
