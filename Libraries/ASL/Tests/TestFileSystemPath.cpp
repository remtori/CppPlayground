#include <catch2/catch2.hpp>

#include <ASL/FileSystemPath.h>

TEST_CASE("FileSystemPath")
{
    SECTION("Absolute path")
    {
        FileSystemPath r("/abc/def/ghi.xyz.zip");
        REQUIRE(r.is_absolute());
        REQUIRE(r.root() == "/");
        REQUIRE(r.dirname() == "/abc/def");
        REQUIRE(r.basename() == "ghi.xyz.zip");
        REQUIRE(r.filename() == "ghi.xyz");
        REQUIRE(r.extension() == ".zip");
    }

    SECTION("Relative path")
    {
        FileSystemPath r("./abc/def/ghi.xyz");
        REQUIRE(!r.is_absolute());
        REQUIRE(r.root().is_empty());
        REQUIRE(r.dirname() == "./abc/def");
        REQUIRE(r.basename() == "ghi.xyz");
        REQUIRE(r.filename() == "ghi");
        REQUIRE(r.extension() == ".xyz");
    }

    SECTION("No extension")
    {
        FileSystemPath r("abc/def/ghi");
        REQUIRE(!r.is_absolute());
        REQUIRE(r.root().is_empty());
        REQUIRE(r.dirname() == "abc/def");
        REQUIRE(r.basename() == "ghi");
        REQUIRE(r.filename() == "ghi");
        REQUIRE(r.extension().is_empty());
    }

    SECTION("Weird path")
    {
        FileSystemPath r("./../../xcc/fff/op.gg");
        REQUIRE(!r.is_absolute());
        REQUIRE(r.root().is_empty());
        REQUIRE(r.dirname() == "./../../xcc/fff");
        REQUIRE(r.basename() == "op.gg");
        REQUIRE(r.filename() == "op");
        REQUIRE(r.extension() == ".gg");
    }

    SECTION("dot")
    {
        FileSystemPath r(".");
        REQUIRE(!r.is_absolute());
        REQUIRE(r.root().is_empty());
        REQUIRE(r.dirname().is_empty());
        REQUIRE(r.basename() == ".");
        REQUIRE(r.filename() == ".");
        REQUIRE(r.extension().is_empty());

        FileSystemPath empty("");
        REQUIRE(empty.root().is_empty());
        REQUIRE(empty.dirname().is_empty());
        REQUIRE(empty.basename().is_empty());
        REQUIRE(empty.filename().is_empty());
        REQUIRE(empty.extension().is_empty());
    }
}
