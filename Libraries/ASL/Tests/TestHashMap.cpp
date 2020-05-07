#include <catch2/catch2.hpp>

#include <ASL/HashMap.h>

TEST_CASE("HashMap", "[container]")
{
    SECTION("Basic usage")
    {
        HashMap<int, char> a;
        a.set(1, 'h');
        a.set(2, 'e');
        a.set(3, 'l');

        REQUIRE(!a.is_empty());
        REQUIRE(a.size() == 3);
        REQUIRE(a.get(1).value() == 'h');
        REQUIRE(a.get(2).value() == 'e');
        REQUIRE(a.get(3).value() == 'l');
    }

    SECTION("Iterator")
    {
        HashMap<int, char> a;
        a.set(1, '1');
        a.set(2, 'e');
        a.set(3, 'l');

        auto it = a.find(2);
        REQUIRE(it != a.end());
        a.remove(it);

        REQUIRE(a.size() == 2);
        REQUIRE(!a.get(2).has_value());

        a.remove(3);
        REQUIRE(a.size() == 1);
        REQUIRE(a.get(1).value() == '1');

        a.set(2, '2');
        a.set(3, '3');
        a.set(4, '4');

        for (auto& kv : a) {
            REQUIRE(('0' + kv.key) == kv.value);
        }
    }
}
