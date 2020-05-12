#include <catch2/catch2.hpp>

#include <ASL/LogStream.h>
#include <ASL/StringFormat.h>

TEST_CASE("String Format", "[string]")
{
    SECTION("basic")
    {
        REQUIRE(format("Hello, world!") == "Hello, world!");
        REQUIRE(format("Some string with arg {}", 1) == "Some string with arg 1");
        REQUIRE(format("Try u32 {}", 70000u) == "Try u32 70000");
        REQUIRE(format("Floating point {}", 0.005f) == "Floating point 0.0050");
        REQUIRE(format("Double point {}", 0.006) == "Double point 0.0060");
    }

    SECTION("multiple argument")
    {
        REQUIRE(
            format("Multiple, {} arguments {} -> {}{}", (char*)"like 3", 3.04, 765432, 'A')
            == "Multiple, like 3 arguments 3.0400 -> 765432A");

        REQUIRE(
            format("A few argument, like {0}, yup {0}", 2)
            == "A few argument, like 2, yup 2");
    }

    SECTION("floating precision")
    {
        REQUIRE(format("{:.2}", 23.1234) == "23.12");
        REQUIRE(format("{:.5}", 23.1234) == "23.12340");
    }

    SECTION("padding")
    {
        REQUIRE(format("{:6}", 123) == "   123");
        REQUIRE(format("{:4}", 123) == " 123");
        REQUIRE(format("{:2}", 123) == "123");
        REQUIRE(format("{:-6}", 123) == "123   ");
        REQUIRE(format("{:-4}", 123) == "123 ");
        REQUIRE(format("{:-2}", 123) == "123");
    }

    SECTION("A complex format")
    {
        REQUIRE(format("{0} {0} {0}, {1}, I'm quite {{complexxx}}, like this number {2:.6} hmm {2:-5.2}", (char*)"yo!", (char*)"Beware", 2.0 / 3.0)
            == "yo! yo! yo!, Beware, I'm quite {{complexxx}}, like this number 0.666667 hmm 0.67 ");
    }
}
