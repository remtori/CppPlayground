#include <catch2/catch2.hpp>

#include <ASL/File.h>

String read_str(String filename)
{
    File fi(filename);
    fi.open(File::Read);
    return fi.read_all();
}

TEST_CASE("File IO")
{
    String tmp_file = "./bin-int/a.txt";
    SECTION("Read File")
    {
        File f("README.md");
        f.open(File::Read);
        REQUIRE(f.is_open());
        String str(f.read(100));
        REQUIRE(str.starts_with("# CppPlayground"));
    }

    SECTION("Write File")
    {
        File fo(tmp_file);
        fo.open(File::Write);
        REQUIRE(fo.is_open());
        fo.write("abcdef");
        fo.close();
        REQUIRE(fo.is_closed());
        REQUIRE(read_str(tmp_file) == "abcdef");

        File foa(tmp_file);
        foa.open(File::Write | File::Append);
        REQUIRE(fo.is_open());
        foa.write("ghi");
        foa.close();
        REQUIRE(fo.is_closed());
        REQUIRE(read_str(tmp_file) == "abcdefghi");

        File fot(tmp_file);
        fot.open(File::Write | File::Truncate);
        REQUIRE(fo.is_open());
        fot.write("ghi");
        fot.close();
        REQUIRE(fo.is_closed());
        REQUIRE(read_str(tmp_file) == "ghi");
    }
}
