#include <ASL/Optinal.h>
#include <catch2/catch2.hpp>
#include <string>
#include <vector>

TEST_CASE("Optional")
{
    SECTION("Optional construct and use")
    {
        Optional<int> x;
        REQUIRE(!x.has_value());
        x = 3;
        REQUIRE(x.has_value());
        REQUIRE(x.value() == 3);
    }

    SECTION("Optional move")
    {
        Optional<int> x;
        REQUIRE(!x.has_value());
        x = 3;
        REQUIRE(x.has_value());
        REQUIRE(x.value() == 3);

        Optional<int> y;
        y = move(x);
        REQUIRE(y.has_value());
        REQUIRE(y.value() == 3);
        REQUIRE(!x.has_value());
    }

    SECTION("Optional leak 1")
    {
        struct Structure {
            Optional<std::string> str;
        };

        std::vector<Structure> vec;
        vec.push_back({ "foo" });
        REQUIRE(vec[0].str.has_value() == true);
        REQUIRE(vec[0].str.value() == "foo");
    }
}
