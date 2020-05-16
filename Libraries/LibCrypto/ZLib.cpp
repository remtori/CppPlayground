#include "ZLib.h"

#include <ASL/LogStream.h>

namespace Crypto {

void zlib_inflate(const BufferStream& input, BufferStream& output)
{
}

void zlib_deflate(const BufferStream& input, BufferStream& output)
{
    bool is_last_block = input.peek() & 0x80;
    u8 compression_type = input.peek() & 0b01100000;
    if (compression_type == 0b01100000) {
        dbg() << "ZLib Delate: Error compression type";
    }

    if (compression_type == 0b00100000) {
        // Compress with fixed Huffman codes
    } else if (compression_type == 0b01100000) {
        // Compress with dynamic Huffman codes
    } else {
        // No Compression
        input.skip(1);

        u16 len;
        input >> len;
        output.write_raw(input.data() + input.offset(), len);
        input.skip(len);
    }
}

} // namespace Crypto
