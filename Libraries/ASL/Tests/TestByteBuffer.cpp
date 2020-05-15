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

    SECTION("Append")
    {
        ByteBuffer a;
        a.append("aa", 2);
        REQUIRE(String(a) == "aa");
        a.append("bcd", 3);
        REQUIRE(String(a) == "aabcd");
    }

    SECTION("Fill")
    {
        ByteBuffer a(4);
        a.fill('f');
        REQUIRE(String(a) == "ffff");

        a.fill('d', 2);
        REQUIRE(String(a) == "ddff");

        a.fill('e', 1, 3);
        REQUIRE(String(a) == "ddfe");
    }

    SECTION("Overwrite")
    {
        ByteBuffer a = ByteBuffer::from_data("eeee");

        a.overwrite((void*)"abc", 3);
        REQUIRE(String(a) == "abce");

        a.overwrite((void*)"abc", 3, 1);
        REQUIRE(String(a) == "aabc");
    }

    SECTION("Sub Equals")
    {
        ByteBuffer a = ByteBuffer::from_data("abcdef");
        REQUIRE(a.sub_equals("abc", 3));
        REQUIRE(a.sub_equals("bcdef", 5, 1));
        REQUIRE(a.sub_equals("cd", 2, 2));
    }
}
