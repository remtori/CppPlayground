#include "CRC.h"

namespace Crypto {

static u32* crc_table = nullptr;

void generate_crc_table()
{
    crc_table = new u32[256];
    u32 POLYNOMIAL = 0xEDB88320;

    u32 c;
    for (u32 n = 0; n <= 255; ++n) {
        c = n;
        for (u8 k = 0; k <= 7; ++k) {
            if (c & 1)
                c = POLYNOMIAL ^ ((c >> 1) & 0x7FFFFFFF);
            else
                c = (c >> 1) & 0x7FFFFFFF;
        }
        crc_table[n] = c;
    }
}

u32 compute_crc_32(const void* data, size_t length)
{
    if (crc_table == nullptr)
        generate_crc_table();

    const u8* bytes = (u8*)data;

    u32 crc32 = 0xFFFFFFFF;
    for (size_t i = 0; i < length; ++i) {
        u8 look_up_index = (crc32 ^ bytes[i]) & 0xff;
        crc32 = (crc32 >> 8) ^ crc_table[look_up_index];
    }

    crc32 ^= 0xFFFFFFFF;
    return crc32;
}

} // namespace Crypto
