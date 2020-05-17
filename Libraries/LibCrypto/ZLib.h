#pragma once

#include <ASL/Forward.h>
#include <ASL/BufferStream.h>

namespace Crypto {

void LIB_API zlib_inflate(const BufferStream& input, BufferStream& output);
void LIB_API zlib_deflate(const BufferStream& input, BufferStream& output);

} // namespace Crypto
