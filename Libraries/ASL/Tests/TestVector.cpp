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
        for (const std::string& str : strings) {
            REQUIRE(!str.empty());
            ++loop_counter;
        }

        loop_counter = 0;
        for (auto& str : strings) {
            REQUIRE(!str.empty());
            ++loop_counter;
        }
        REQUIRE(loop_counter == 2);
    }

    SECTION("Remove")
    {
        Vector<int> ints;
        ints.append(1);
        ints.append(2);
        ints.append(3);
        ints.append(4);
        ints.append(5);

        ints.remove(1);
        REQUIRE(ints.size() == 4u);
        REQUIRE(ints[0] == 1);
        REQUIRE(ints[1] == 3);
        REQUIRE(ints[2] == 4);
        REQUIRE(ints[3] == 5);

        ints.remove(0);
        REQUIRE(ints.size() == 3u);
        REQUIRE(ints[0] == 3);
        REQUIRE(ints[1] == 4);
        REQUIRE(ints[2] == 5);

        ints.take_back();
        REQUIRE(ints.size() == 2u);
        REQUIRE(ints[0] == 3);
        REQUIRE(ints[1] == 4);

        ints.take_front();
        REQUIRE(ints.size() == 1u);
        REQUIRE(ints[0] == 4);
    }

    SECTION("Insert trivial")
    {
        Vector<int> ints;
        ints.append(0);
        ints.append(10);
        ints.append(20);
        ints.append(30);
        ints.append(40);
        ints.insert(2, 15);
        REQUIRE(ints.size() == 6u);
        REQUIRE(ints[0] == 0);
        REQUIRE(ints[1] == 10);
        REQUIRE(ints[2] == 15);
        REQUIRE(ints[3] == 20);
        REQUIRE(ints[4] == 30);
        REQUIRE(ints[5] == 40);
    }

    SECTION("Append")
    {
        Vector<char> str;
        str.append('a');
        str.append('b');
        str.append("cdef", 4);
        REQUIRE(str[0] == 'a');
        REQUIRE(str[1] == 'b');
        REQUIRE(str[2] == 'c');
        REQUIRE(str[3] == 'd');
        REQUIRE(str[4] == 'e');
        REQUIRE(str[5] == 'f');
    }

    SECTION("Compare")
    {
        Vector<int> ints;
        Vector<int> same_ints;

        for (int i = 0; i < 1000; ++i) {
            ints.append(i);
            same_ints.append(i);
        }

        REQUIRE(ints.size() == 1000u);
        REQUIRE(ints == same_ints);

        Vector<std::string> strings;
        Vector<std::string> same_strings;

        for (int i = 0; i < 1000; ++i) {
            strings.append(std::to_string(i));
            same_strings.append(std::to_string(i));
        }

        REQUIRE(strings.size() == 1000u);
        REQUIRE(strings == same_strings);
    }

    SECTION("Insert string ordered")
    {
        Vector<std::string> strings;
        strings.append("abc");
        strings.append("def");
        strings.append("ghi");

        strings.insert_before_matching("f-g", [](auto& entry) {
            return "f-g" < entry;
        });

        REQUIRE(strings[0] == "abc");
        REQUIRE(strings[1] == "def");
        REQUIRE(strings[2] == "f-g");
        REQUIRE(strings[3] == "ghi");
    }

    SECTION("Prepend")
    {
        Vector<int> ints;
        ints.append(1);
        ints.append(2);
        ints.append(3);

        Vector<int> more_ints;
        more_ints.append(4);
        more_ints.append(5);
        more_ints.append(6);

        ints.prepend(move(more_ints));

        REQUIRE(ints.size() == 6u);
        REQUIRE(more_ints.size() == 0u);

        REQUIRE(ints[0] == 4);
        REQUIRE(ints[1] == 5);
        REQUIRE(ints[2] == 6);
        REQUIRE(ints[3] == 1);
        REQUIRE(ints[4] == 2);
        REQUIRE(ints[5] == 3);

        ints.prepend(move(more_ints));
        REQUIRE(ints.size() == 6u);
        REQUIRE(more_ints.size() == 0u);

        more_ints.prepend(move(ints));
        REQUIRE(more_ints.size() == 6u);
        REQUIRE(ints.size() == 0u);
    }
}
