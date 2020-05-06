#include <catch2/catch2.hpp>

#include <ASL/Vector.h>
#include <string>

TEST_CASE("Vector", "[container]")
{

    SECTION("construct")
    {
        REQUIRE(Vector<int>().is_empty());
        REQUIRE(Vector<int>().size() == 0);
    }

    SECTION("Usage with int")
    {
        Vector<int> ints;
        ints.append(1);
        ints.append(2);
        ints.append(3);
        REQUIRE(ints.size() == 3u);
        REQUIRE(ints.take_back() == 3);
        REQUIRE(ints.size() == 2u);
        REQUIRE(ints.take_back() == 2);
        REQUIRE(ints.size() == 1u);
        REQUIRE(ints.take_back() == 1);
        REQUIRE(ints.size() == 0u);

        ints.clear();
        REQUIRE(ints.size() == 0u);
    }

    SECTION("Useage with std::string")
    {
        Vector<std::string> strings;
        strings.append("ABC");
        strings.append("DEF");

        int loop_counter = 0;
        for (size_t i = 0; i < strings.size(); i++) {
            REQUIRE(!strings[i].empty());
            ++loop_counter;
        }

        loop_counter = 0;
        for (size_t i = 0; i < strings.size(); i++) {
            REQUIRE(!strings[i].empty());
            ++loop_counter;
        }
        REQUIRE(loop_counter == 2);
    }
}
