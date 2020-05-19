#include <catch2/catch2.hpp>

#include <ASL/StringBuilder.h>

TEST_CASE("String builder", "[string]")
{
    SECTION("Build string")
    {
        StringBuilder builder;
        builder.append("hello");
        builder.append_repeated(' ', 2);
        builder.append("world!");
        REQUIRE(builder.to_string() == "hello  world!");
    }

    SECTION("Build string formatted")
    {
        StringBuilder builder;
        builder.append("hi");
        builder.append(' ');
        builder.append_formatted("{0}, i can show you the {0}", (char*)"world");
        builder.append_repeated(' ', 2);
        builder.append_formatted("{:2} {:2}", 1, 10);
        REQUIRE(builder.to_string() == "hi world, i can show you the world   1 10");
    }
}
