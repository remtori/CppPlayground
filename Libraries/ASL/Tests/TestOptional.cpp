#include <catch2/catch2.hpp>

#include <ASL/LogStream.h>
#include <ASL/Optional.h>
#include <ASL/String.h>
#include <ASL/Vector.h>

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
            Optional<String> str;
        };

        Vector<Structure> vec;
        vec.append({ "foo" });
        REQUIRE(vec[0].str.has_value() == true);
        REQUIRE(vec[0].str.value() == "foo");
    }
}
