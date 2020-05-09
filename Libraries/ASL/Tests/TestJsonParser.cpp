#include <catch2/catch2.hpp>

#include <ASL/JsonParser.h>

TEST_CASE("JsonParser", "[parser]")
{
    SECTION("parse string")
    {
        JsonParser parser("\"Hello, World!\"");
        auto v = parser.parse();
        REQUIRE(v.is_string());
        REQUIRE(v.as_string() == "Hello, World!");
    }

    SECTION("parse number")
    {
        JsonParser p_a("1234");
        auto v_a = p_a.parse();
        REQUIRE(v_a.is_u32());
        REQUIRE(v_a.as_u32() == 1234);

        JsonParser p_b("-123");
        auto v_b = p_b.parse();
        REQUIRE(v_b.is_i32());
        REQUIRE(v_b.as_i32() == -123);

        JsonParser p_c("-3.14");
        auto v_c = p_c.parse();
        REQUIRE(v_c.is_double());
        REQUIRE(v_c.as_double() == -3.14);

        // JsonParser p_d("0x123");
        // auto v_d = p_d.parse();
        // REQUIRE(v_d.is_null());
    }

    SECTION("Parse object")
    {
        JsonParser parser("{\"a\": 1, \"b\": -2.3, \"c\": \"hello, world!\"}");
        auto value = parser.parse();
        REQUIRE(value.is_object());

        auto obj = value.as_object();
        REQUIRE(obj.get("a").is_u32());
        REQUIRE(obj.get("a").as_u32() == 1);
        REQUIRE(obj.get("b").is_double());
        REQUIRE(obj.get("b").as_double() == -2.3);
        REQUIRE(obj.get("c").is_string());
        REQUIRE(obj.get("c").as_string() == "hello, world!");
    }

    SECTION("Parse array")
    {
        JsonParser parser("[ 1, 2.0, -3, \"hello\" ]");
        auto value = parser.parse();
        REQUIRE(value.is_array());

        auto arr = value.as_array();
        REQUIRE(arr[0].is_u32());
        REQUIRE(arr[0].as_u32() == 1);
        REQUIRE(arr[1].is_double());
        REQUIRE(arr[1].as_double() == 2.0);
        REQUIRE(arr[2].is_i32());
        REQUIRE(arr[2].as_i32() == -3);
        REQUIRE(arr[3].is_string());
        REQUIRE(arr[3].as_string() == "hello");
    }
}