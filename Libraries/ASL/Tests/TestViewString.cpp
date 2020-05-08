#include <catch2/catch2.hpp>

#include <ASL/String.h>
#include <ASL/StringView.h>

TEST_CASE("String View", "[string]")
{
    SECTION("Empty")
    {
        REQUIRE(StringView().is_null());
        REQUIRE(StringView().is_empty());
        REQUIRE(!StringView().characters_wont());
        REQUIRE(StringView().length() == 0u);
    }

    SECTION("View literal")
    {
        const char* truth = "cats rule dogs drool";
        StringView view(truth);
        REQUIRE(view.is_null() == false);
        REQUIRE(view.characters_wont() == truth);
        REQUIRE(view == view);
        REQUIRE(view == truth);
    }

    SECTION("Compare view")
    {
        String foo1 = "foo";
        String foo2 = "foo";
        auto view1 = foo1.view();
        auto view2 = foo2.view();

        REQUIRE(view1 == view2);
        REQUIRE(view1 == foo1);
        REQUIRE(view1 == foo2);
        REQUIRE(view1 == "foo");
    }

    SECTION("Starts with")
    {
        String test_string = "ABCDEF";
        StringView test_string_view = test_string.view();
        REQUIRE(test_string_view.starts_with('A'));
        REQUIRE(!test_string_view.starts_with('B'));
        REQUIRE(test_string_view.starts_with("AB"));
        REQUIRE(test_string_view.starts_with("ABCDEF"));
        REQUIRE(!test_string_view.starts_with("DEF"));
    }

    SECTION("Ends with")
    {
        String test_string = "ABCDEF";
        StringView test_string_view = test_string.view();
        REQUIRE(test_string_view.ends_with("DEF"));
        REQUIRE(test_string_view.ends_with('F'));
        REQUIRE(!test_string_view.ends_with('E'));
        REQUIRE(test_string_view.ends_with("ABCDEF"));
        REQUIRE(!test_string_view.ends_with("ABCDE"));
        REQUIRE(!test_string_view.ends_with("ABCDEFG"));
    }

    SECTION("Find first of")
    {
        String test_string = "aabbcc_xy_ccbbaa";
        StringView test_string_view = test_string.view();

        REQUIRE(test_string_view.find_first_of('b').has_value() == true);
        REQUIRE(test_string_view.find_first_of('b').value() == 2U);

        REQUIRE(test_string_view.find_first_of('_').has_value() == true);
        REQUIRE(test_string_view.find_first_of('_').value() == 6U);

        REQUIRE(test_string_view.find_first_of("bc").has_value() == true);
        REQUIRE(test_string_view.find_first_of("bc").value() == 2U);

        REQUIRE(test_string_view.find_first_of("yx").has_value() == true);
        REQUIRE(test_string_view.find_first_of("yx").value() == 7U);

        REQUIRE(test_string_view.find_first_of('n').has_value() == false);
        REQUIRE(test_string_view.find_first_of("defg").has_value() == false);
    }

    SECTION("Find last of")
    {
        String test_string = "aabbcc_xy_ccbbaa";
        StringView test_string_view = test_string.view();

        REQUIRE(test_string_view.find_last_of('b').has_value() == true);
        REQUIRE(test_string_view.find_last_of('b').value() == 13U);

        REQUIRE(test_string_view.find_last_of('_').has_value() == true);
        REQUIRE(test_string_view.find_last_of('_').value() == 9U);

        REQUIRE(test_string_view.find_last_of("bc").has_value() == true);
        REQUIRE(test_string_view.find_last_of("bc").value() == 13U);

        REQUIRE(test_string_view.find_last_of("yx").has_value() == true);
        REQUIRE(test_string_view.find_last_of("yx").value() == 8U);

        REQUIRE(test_string_view.find_last_of('3').has_value() == false);
        REQUIRE(test_string_view.find_last_of("fghi").has_value() == false);
    }
}
