#include <catch2/catch2.hpp>

#include <ASL/StdLibExtras.h>
#include <ASL/Types.h>

struct A {
    A(int& v)
        : value(v)
    {
    }

    A(int&& v)
        : move_v(v)
    {
    }

    int value = 0;
    int move_v = 0;
};

TEST_CASE("StdLibExtras")
{
    SECTION("min")
    {
        REQUIRE(min(2, 3) == 2);
        REQUIRE(min<float>(1.0, 9.0f) == 1.0f);
        REQUIRE(min(10000000000, (i64)1) == 1);
    }

    SECTION("max")
    {
        REQUIRE(max(2, 3) == 3);
        REQUIRE(max<float>(1.0, 9.0f) == 9.0f);
        REQUIRE(max(10000000000, (i64)1) == 10000000000);
    }

    SECTION("move")
    {
        int a = 2;
        A move_a(move(a));
        REQUIRE(move_a.value == 0);
        REQUIRE(move_a.move_v == a);

        A b(a);
        REQUIRE(b.value == a);
        REQUIRE(b.move_v == 0);

        A c(4);
        REQUIRE(c.value == 0);
        REQUIRE(c.move_v == 4);
    }
}
