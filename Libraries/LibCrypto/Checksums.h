#pragma once

#include <ASL/Forward.h>
#include <ASL/Types.h>

namespace Crypto {

u32 LIB_API update_adler32(u32 adler, const void* data, size_t length);
u32 LIB_API compute_adler32(const void* data, size_t length);
u32 LIB_API compute_crc32(const void* data, size_t length);

} // namespace Crypto
