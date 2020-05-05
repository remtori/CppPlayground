#include <catch2/catch2.hpp>

#include <ASL/NonnullOwnPtr.h>
#include <ASL/OwnPtr.h>

struct A {
    int v;
    A(int a)
        : v(a)
    {
    }
};

TEST_CASE("OwnPtr and NonnullOwnPtr", "[smart_ptr]")
{
    SECTION("Construct and using OwnPtr")
    {
        OwnPtr<A> a(new A(4));
        REQUIRE(a->v == 4);
        REQUIRE((*a).v == 4);
        REQUIRE(a.ptr()->v == 4);
        REQUIRE((bool)a);

        a.clear();
        REQUIRE(!a);
        REQUIRE(!a.has_ptr());
        REQUIRE(((bool)a) == false);
    }

    SECTION("Construct and using NonnullOwnPtr")
    {
        NonnullOwnPtr<A> a = create_own<A>(5);
        REQUIRE(a->v == 5);
        REQUIRE((*a).v == 5);
        REQUIRE(a.ptr()->v == 5);
        REQUIRE((bool)a);
    }

    SECTION("Moving between ptr")
    {
        OwnPtr<A> a = create_own<A>(2);
        auto b = move(a);
        REQUIRE(!a.has_ptr());
        REQUIRE(b.has_ptr());
        REQUIRE(b->v == 2);

        auto c = b.release_nonnull();
        REQUIRE(!b.has_ptr());
        REQUIRE(c->v == 2);
    }
}
