#include <catch2/catch2.hpp>

#include <LibCrypto/CRC.h>

TEST_CASE("CRC", "[check_sum]")
{
    SECTION("CRC32")
    {
        REQUIRE(Crypto::compute_crc_32((char*)"abc", 3) == 0x352441c2);
        REQUIRE(Crypto::compute_crc_32((char*)"lqv", 3) == 0x31ce77e8);
        REQUIRE(Crypto::compute_crc_32((char*)"remtori", 7) == 0x51b14a37);
    }
}
