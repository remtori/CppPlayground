#include <catch2/catch2.hpp>

#include <ASL/ByteBuffer.h>
#include <ASL/String.h>

TEST_CASE("ByteBuffer", "[container]")
{
    SECTION("Basic usage")
    {
        ByteBuffer a(3);
        a[0] = 'a';
        a[1] = 'b';
        a[2] = 'c';

        REQUIRE(String(a) == "abc");
    }

    SECTION("Fill")
    {
        ByteBuffer b(4);
        b.fill('f');
        REQUIRE(String(b) == "ffff");

        b.fill('d', 2);
        REQUIRE(String(b) == "ddff");

        b.fill('e', 2, 3);
        REQUIRE(String(b) == "ddfe");
    }

    SECTION("Copy from")
    {
        ByteBuffer b = ByteBuffer::from_data("eeee");

        b.copy_from((void*)"abc", 3);
        REQUIRE(String(b) == "abce");

        b.copy_from((void*)"abc", 3, 1);
        REQUIRE(String(b) == "aabc");
    }
}
