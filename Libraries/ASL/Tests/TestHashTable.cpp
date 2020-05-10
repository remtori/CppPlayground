#include <catch2/catch2.hpp>

#include <ASL/HashTable.h>

TEST_CASE("HashTable", "[container]")
{
    SECTION("Basic usage")
    {
        HashTable<int> a;
        a.set(4);
        a.set(5);
        a.set(6);
        REQUIRE(!a.is_empty());
        REQUIRE(a.size() == 3);
        REQUIRE(a.contains(4));
        REQUIRE(a.contains(5));
        REQUIRE(a.contains(6));
    }

    SECTION("Iterating")
    {
        HashTable<int> a;
        a.set(4);
        a.set(5);
        a.set(6);

        for (auto& v : a) {
            REQUIRE(a.contains(v));
        }

        for (const auto& v : a) {
            REQUIRE(a.contains(v));
        }
    }

    SECTION("Find and remove")
    {
        HashTable<int> a;
        a.set(4);
        a.set(5);
        a.set(6);

        auto it = a.find(5);
        a.remove(it);
        a.remove(6);

        REQUIRE(a.size() == 1);
        REQUIRE((*a.begin()) == 4);
    }

    SECTION("Preloccate")
    {
        HashTable<int> a;
        a.ensure_capacity(100);
        REQUIRE(a.size() == 0);

        for (int i = 0; i < 100; ++i)
            a.set(i);

        REQUIRE(a.size() == 100);
    }
}
