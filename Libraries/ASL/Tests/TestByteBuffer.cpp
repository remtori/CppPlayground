#include <catch2/catch2.hpp>

#include <ASL/ByteBuffer.h>
#include <string>

TEST_CASE("ByteBuffer", "[container]")
{
    SECTION("Basic usage")
    {
        ByteBuffer a(3);
        a[0] = 'a';
        a[1] = 'b';
        a[2] = 'c';
        REQUIRE(std::string((char*)a.data(), a.size()) == "abc");
    }

    SECTION("Append")
    {
        ByteBuffer a;
        a.append("aa", 2);
        REQUIRE(std::string((char*)a.data(), 2) == "aa");
        a.append("bcd", 3);
        REQUIRE(std::string((char*)a.data(), 5) == "aabcd");
    }

    SECTION("Fill")
    {
        ByteBuffer a(4);
        a.fill('f');
        REQUIRE(std::string((char*)a.data(), a.size()) == "ffff");

        a.fill('d', 2);
        REQUIRE(std::string((char*)a.data(), a.size()) == "ddff");

        a.fill('e', 1, 3);
        REQUIRE(std::string((char*)a.data(), a.size()) == "ddfe");
    }

    SECTION("Overwrite")
    {
        ByteBuffer a = ByteBuffer::from_data("eeee");

        a.overwrite((void*)"abc", 3);
        REQUIRE(std::string((char*)a.data(), a.size()) == "abce");

        a.overwrite((void*)"abc", 3, 1);
        REQUIRE(std::string((char*)a.data(), a.size()) == "aabc");
    }

    SECTION("Sub Equals")
    {
        ByteBuffer a = ByteBuffer::from_data("abcdef");
        REQUIRE(a.sub_equals("abc", 3));
        REQUIRE(a.sub_equals("bcdef", 5, 1));
        REQUIRE(a.sub_equals("cd", 2, 2));
    }

    SECTION("Read")
    {
        ByteBuffer a = ByteBuffer::from_data("abcdef");
        char str[3];
        a.read(str, 3);
        REQUIRE(std::string((char*)str, 3) == "abc");
        a.read(str, 3, 2);
        REQUIRE(std::string((char*)str, 3) == "cde");
    }
}