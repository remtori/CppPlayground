#include <catch2/catch2.hpp>

#include <ASL/ByteStream.h>
#include <ASL/String.h>

TEST_CASE("ByteStream")
{
    SECTION("R/W stuff")
    {
        ByteStream stream;
        stream << (u8)3 << (u16)1234 << 66666u << 12345678901u << (i8)-2 << (i16)-265 << "abcdef";
        i8 v8;
        i16 v16;
        i32 v32;
        i64 v64;
        stream.reset();

        stream >> v8;
        REQUIRE(v8 == 3);
        stream >> v16;
        REQUIRE(v16 == 1234);
        stream >> v32;
        REQUIRE(v32 == 66666);
        stream >> v64;
        REQUIRE(v64 == 12345678901);
        stream >> v8;
        REQUIRE(v8 == -2);
        stream >> v16;
        REQUIRE(v16 == -265);

        char c[7];
        c[6] = 0;

        stream.read((u8*)c, 6);
        REQUIRE(String(c) == "abcdef");
    }
}
