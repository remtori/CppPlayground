#include <catch2/catch2.hpp>

#include <ASL/String.h>

TEST_CASE("String", "[string]")
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
        REQUIRE(a.starts_with("AB"));
        REQUIRE(a.starts_with("ABCDEF"));
        REQUIRE(!a.starts_with("DEF"));

        REQUIRE(a.starts_with('a', false));
        REQUIRE(!a.starts_with('b', false));
        REQUIRE(a.starts_with("ab", false));
        REQUIRE(a.starts_with("abcdef", false));
        REQUIRE(!a.starts_with("def", false));
    }

    SECTION("Ends With")
    {
        String a = "ABCDEF";
        REQUIRE(a.ends_with('F'));
        REQUIRE(!a.ends_with('E'));
        REQUIRE(a.ends_with("EF"));
        REQUIRE(a.ends_with("ABCDEF"));
        REQUIRE(!a.ends_with("ABC"));

        REQUIRE(a.ends_with('f', false));
        REQUIRE(!a.ends_with('e', false));
        REQUIRE(a.ends_with("ef", false));
        REQUIRE(a.ends_with("abcdef", false));
        REQUIRE(!a.ends_with("abc", false));
    }

    SECTION("Equals")
    {
        String a = "ABCDEF";
        REQUIRE(a.equals("ABCDEF"));
        REQUIRE(!a.equals("ABCDE"));
        REQUIRE(a.equals("abcdef", false));
        REQUIRE(!a.equals("abcde", false));
    }

    SECTION("Split")
    {
        String test = "foo bar baz";
        auto parts = test.split(' ');

        REQUIRE(parts.size() == 3u);
        REQUIRE(parts[0] == "foo");
        REQUIRE(parts[1] == "bar");
        REQUIRE(parts[2] == "baz");

        REQUIRE(parts[0].characters()[3] == '\0');
        REQUIRE(parts[1].characters()[3] == '\0');
        REQUIRE(parts[2].characters()[3] == '\0');
    }

    SECTION("To number")
    {
        REQUIRE(String("34343").to_uint() == 34343u);
        REQUIRE(String("+12312").to_uint() == 12312u);
        REQUIRE(String("-34343").to_int() == -34343);
        REQUIRE(String("12334343").to_int() == 12334343);
        REQUIRE(String("-3.14").to_double() == -3.14);

        bool ok;

        String("-1").to_uint(&ok);
        REQUIRE(!ok);

        String("--1").to_int(&ok);
        REQUIRE(!ok);

        String("abc").to_int(&ok);
        REQUIRE(!ok);

        String("333.3.3").to_double(&ok);
        REQUIRE(!ok);

        String("-3-4.234").to_double(&ok);
        REQUIRE(!ok);
    }

    SECTION("Find")
    {
        String a = "ABBCDEEF";

        {
            auto v = a.index_of("B");
            REQUIRE(v.has_value());
            REQUIRE(v.value() == 1u);
        }
        {
            auto v = a.last_index_of("B");
            REQUIRE(v.has_value());
            REQUIRE(v.value() == 2u);
        }
        {
            auto v = a.index_of("BB");
            REQUIRE(v.has_value());
            REQUIRE(v.value() == 1u);
        }
        {
            auto v = a.index_of("G");
            REQUIRE(!v.has_value());
        }
        {
            auto v = a.index_of("AC");
            REQUIRE(!v.has_value());
        }
        {
            auto v = a.last_index_of("G");
            REQUIRE(!v.has_value());
        }
        {
            auto v = a.last_index_of("AC");
            REQUIRE(!v.has_value());
        }
        {
            auto v = a.index_of('B');
            REQUIRE(v.has_value());
            REQUIRE(v.value() == 1);
        }
        {
            auto v = a.last_index_of('B');
            REQUIRE(v.has_value());
            REQUIRE(v.value() == 2);
        }
    }
}
