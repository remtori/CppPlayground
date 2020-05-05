#include <catch2/catch2.hpp>

#include <ASL/NonnullRefPtr.h>
#include <ASL/RefCounted.h>
#include <ASL/RefPtr.h>
#include <ASL/StdLibExtras.h>

struct Object : public RefCounted<Object> {
    int x;
};

TEST_CASE("RefPtr and NonnullRefPtr", "[smart_ptr]")
{
    struct A : public RefCounted<A> {
    public:
        A(int a)
        {
            v = a;
        }

        int v;
    };

    SECTION("NonnullRefPtr construct and use")
    {
        auto object = adopt(*new Object);
        REQUIRE(object.ptr() != nullptr);
        REQUIRE(object->ref_count() == 1);
        object->ref();
        REQUIRE(object->ref_count() == 2);
        object->unref();
        REQUIRE(object->ref_count() == 1);

        {
            NonnullRefPtr another = object;
            REQUIRE(object->ref_count() == 2);
        }

        REQUIRE(object->ref_count() == 1);
    }

    SECTION("NonnullRefPtr assign reference")
    {
        auto object = adopt(*new Object);
        REQUIRE(object->ref_count() == 1);
        object = *object;
        REQUIRE(object->ref_count() == 1);
    }

    SECTION("NonnullRefPtr assign owner of self")
    {
        struct Object : public RefCounted<Object> {
            RefPtr<Object> parent;
        };

        auto parent = adopt(*new Object);
        auto child = adopt(*new Object);
        child->parent = move(parent);

        child = *child->parent;
        REQUIRE(child->ref_count() == 1);
    }

    SECTION("NonnullRefPtr swap with self")
    {
        auto object = adopt(*new Object);
        swap(object, object);
        REQUIRE(object->ref_count() == 1);
    }

    SECTION("RefPtr construct and use")
    {
        RefPtr<Object> object = adopt(*new Object);
        REQUIRE(object.ptr() != nullptr);
        REQUIRE(object->ref_count() == 1);
        object->ref();
        REQUIRE(object->ref_count() == 2);
        object->unref();
        REQUIRE(object->ref_count() == 1);

        {
            NonnullRefPtr another = *object;
            REQUIRE(object->ref_count() == 2);
        }

        REQUIRE(object->ref_count() == 1);
    }

    SECTION("RefPtr assign reference")
    {
        RefPtr<Object> object = adopt(*new Object);
        REQUIRE(object->ref_count() == 1);
        object = *object;
        REQUIRE(object->ref_count() == 1);
    }

    SECTION("RefPtr assign pointer")
    {
        RefPtr<Object> object = adopt(*new Object);
        REQUIRE(object->ref_count() == 1);
        object = object.ptr();
        REQUIRE(object->ref_count() == 1);
    }

    SECTION("RefPtr move self")
    {
        RefPtr<Object> object = adopt(*new Object);
        REQUIRE(object->ref_count() == 1);
        object = move(object);
        REQUIRE(object->ref_count() == 1);
    }

    SECTION("RefPtr assign copy self")
    {
        RefPtr<Object> object = adopt(*new Object);
        REQUIRE(object->ref_count() == 1);
        object = object;
        REQUIRE(object->ref_count() == 1);
    }
}
