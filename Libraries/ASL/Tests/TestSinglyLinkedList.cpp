#include <catch2/catch2.hpp>

#include <ASL/SinglyLinkedList.h>

TEST_CASE("SinglyLinkedList", "[container]")
{
    SECTION("Basic usage")
    {
        SinglyLinkedList<int> list;
        list.append(10);
        list.append(20);
        list.append(30);
        REQUIRE(!list.is_empty());

        REQUIRE(list.front() == 10);
        REQUIRE(list.back() == 30);
        REQUIRE(*(++list.begin()) == 20);
        REQUIRE(list.size_slow() == 3);
    }

    SECTION("Insert and Iterator")
    {
        SinglyLinkedList<int> list;
        list.append(10);
        list.append(20);
        list.append(30);

        auto it = list.begin();
        list.insert_after(it, 15);
        list.insert_before(it, 5);
        REQUIRE((*it) == 10);
        REQUIRE(list.front() == 5);
        REQUIRE(*(++it) == 15);
        REQUIRE((*(++list.begin())) == 10);
    }

    SECTION("Iterate, contain and clean up")
    {
        SinglyLinkedList<int> list;
        list.append(10);
        list.append(20);
        list.append(30);

        for (int& a : list) {
            REQUIRE(list.contains_slow(a));
        }

        REQUIRE(list.find(20) == (++list.begin()));
        REQUIRE(list.size_slow() == 3);
        list.clear();
        REQUIRE(list.size_slow() == 0);
        REQUIRE(list.is_empty());
    }
}
