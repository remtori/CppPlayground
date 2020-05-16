#include <catch2/catch2.hpp>

#include <LibCrypto/Checksums.h>

TEST_CASE("CRC", "[check_sum]")
{
    SECTION("CRC32")
    {
        REQUIRE(Crypto::compute_crc32((char*)"abc", 3) == 0x352441c2);
        REQUIRE(Crypto::compute_crc32((char*)"lqv", 3) == 0x31ce77e8);
        REQUIRE(Crypto::compute_crc32((char*)"remtori", 7) == 0x51b14a37);
    }

    SECTION("Adler32")
    {
        REQUIRE(Crypto::compute_adler32((char*)"abc", 3) == 0x024d0127);
        REQUIRE(Crypto::compute_adler32((char*)"lqv", 3) == 0x029f0154);
        REQUIRE(Crypto::compute_adler32((char*)"remtori", 7) == 0x0c0e0303);

        u32 adler = Crypto::compute_adler32((char*)"rem", 3);
        REQUIRE(Crypto::update_adler32(adler, (char*)"tori", 4) == 0x0c0e0303);
    }
}
