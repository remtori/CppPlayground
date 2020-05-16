#pragma once

#include <ASL/Types.h>

namespace Crypto {

u32 update_adler32(u32 adler, const void* data, size_t length);
u32 compute_adler32(const void* data, size_t length);
u32 compute_crc32(const void* data, size_t length);

} // namespace Crypto
