#include <catch2/catch2.hpp>

#include <ASL/String.h>

#include <string.h>

TEST_CASE("String", "[container]")
{
    SECTION("Empty String")
    {
        REQUIRE(String().is_null());
        REQUIRE(String().is_empty());
        REQUIRE(!String().characters());

        REQUIRE(!String("").is_null());
        REQUIRE(String("").is_empty());
        REQUIRE(String("").characters() != nullptr);

        REQUIRE(String("").impl() == String::empty().impl());
    }

    SECTION("Construct content")
    {
        String a = "ABCDEF";
        REQUIRE(!a.is_empty());
        REQUIRE(!a.is_null());
        REQUIRE(a.length() == 6u);
        REQUIRE(a.length() == strlen(a.characters()));
        REQUIRE(a.characters() != nullptr);
        REQUIRE(!strcmp(a.characters(), "ABCDEF"));

        REQUIRE(a == "ABCDEF");
        REQUIRE(a != "ABCDE");
        REQUIRE(a != "ABCDEFG");
    }

    SECTION("Compare")
    {
        REQUIRE("a" < String("b"));
        REQUIRE(!("a" > String("b")));
        REQUIRE("b" > String("a"));
        REQUIRE(!("b" < String("b")));
        REQUIRE("a" >= String("a"));
        REQUIRE(!("a" >= String("b")));
        REQUIRE("a" <= String("a"));
        REQUIRE(!("b" <= String("a")));
        REQUIRE(String("b") > "a");
        REQUIRE(!(String("a") > "b"));
        REQUIRE(String("b") >= "a");
        REQUIRE(!(String("a") >= "b"));
        REQUIRE(String("c") < "d");
        REQUIRE(!(String("d") < "c"));
        REQUIRE(String("c") <= "d");
        REQUIRE(!(String("d") <= "c"));
    }

    SECTION("Index access")
    {
        String a = "ABCDEF";
        REQUIRE(a[0] == 'A');
        REQUIRE(a[1] == 'B');
    }

    SECTION("::repeated()")
    {
        REQUIRE(String::repeated('x', 0) == "");
        REQUIRE(String::repeated('x', 1) == "x");
        REQUIRE(String::repeated('x', 2) == "xx");
    }

    SECTION("Copy String")
    {
        String a = "ABCDEF";
        auto a_copy = a;
        REQUIRE(a == a_copy);
        REQUIRE(a.characters() == a_copy.characters());
    }

    SECTION("Move String")
    {
        String a = "ABCDEF";
        auto a_copy = a;
        auto a_move = move(a_copy);
        REQUIRE(a == a_move);
        REQUIRE(a_copy.is_null());
    }

    SECTION("Starts With")
    {
        String a = "ABCDEF";
        REQUIRE(a.starts_with('A'));
        REQUIRE(!a.starts_with('B'));
        // REQUIRE(a.starts_with("AB"));
        // REQUIRE(a.starts_with("ABCDEF"));
        // REQUIRE(!a.starts_with("DEF"));
    }

    SECTION("Ends With")
    {
        String a = "ABCDEF";
        REQUIRE(a.ends_with('F'));
        REQUIRE(!a.ends_with('E'));
        // REQUIRE(a.ends_with("EF"));
        // REQUIRE(a.ends_with("ABCDEF"));
        // REQUIRE(!a.ends_with("ABC"));
    }
}