#pragma once

#include <ASL/BufferStream.h>

namespace Crypto {

void zlib_inflate(const BufferStream& input, BufferStream& output);
void zlib_deflate(const BufferStream& input, BufferStream& output);

} // namespace Crypto
