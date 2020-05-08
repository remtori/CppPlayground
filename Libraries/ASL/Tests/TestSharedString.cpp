#include <catch2/catch2.hpp>

#include <ASL/SharedString.h>

TEST_CASE("SharedString", "[string]")
{
    SECTION("Basic Usage")
    {
        auto base = String("abc");
        SharedString a = base;
        SharedString b = "abc";
        SharedString c = StringView(base);

        REQUIRE(a.impl() == b.impl());
        REQUIRE(b.impl() == c.impl());
        REQUIRE(a.impl() == c.impl());
    }

    SECTION("Compare")
    {
        SharedString a = "abcdef";
        SharedString b = "abcde";
        SharedString c = "abcdef";
        REQUIRE(a != b);
        REQUIRE(b != c);
        REQUIRE(a == c);
    }
}
