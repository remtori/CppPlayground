#include <LibCrypto/Checksums.h>

namespace Crypto {

u32 update_adler32(u32 adler, const void* data, size_t length)
{
    static const u16 M = 65521;
    u32 A = adler & 0xffff;
    u32 B = (adler >> 16) & 0xffff;

    u8* bytes = (u8*)data;
    for (size_t i = 0; i < length; ++i) {
        A = (A + bytes[i]) % M;
        B = (B + A) % M;
    }

    return (B << 16) | A;
}

u32 compute_adler32(const void* data, size_t length)
{
    return update_adler32(1L, data, length);
}

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

u32 compute_crc32(const void* data, size_t length)
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
