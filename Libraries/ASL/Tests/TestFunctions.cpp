#include <catch2/catch2.hpp>

#include <ASL/Function.h>

TEST_CASE("Function")
{
    Function<int(int)> sqr = [](int a) { return a * a; };
    REQUIRE(sqr(5) == 25);

    Function<int(void)> n5(nullptr);
    n5 = [] { return 5; };
    REQUIRE(n5() == 5);
    n5 = nullptr;
    REQUIRE((bool)n5 == false);
}
