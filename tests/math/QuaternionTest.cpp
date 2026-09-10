#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include "math/Quaternion.h"
#include "math/Vec3.h"

using Catch::Approx;

TEST_CASE("Quaternion identity represents no rotation")
{
    Quaternion q = Quaternion::identity();

    REQUIRE(q.w == Approx(1.0f));
    REQUIRE(q.x == Approx(0.0f));
    REQUIRE(q.y == Approx(0.0f));
    REQUIRE(q.z == Approx(0.0f));
}

TEST_CASE("Quaternion default constructor creates identity")
{
    Quaternion q;

    REQUIRE(q.w == Approx(1.0f));
    REQUIRE(q.x == Approx(0.0f));
    REQUIRE(q.y == Approx(0.0f));
    REQUIRE(q.z == Approx(0.0f));
}

TEST_CASE("Quaternion identity has length one")
{
    Quaternion q = Quaternion::identity();

    REQUIRE(q.length() == Approx(1.0f));
}

TEST_CASE("Quaternion normalized returns unit quaternion")
{
    Quaternion q{2.0f, 2.0f, 2.0f, 2.0f};

    Quaternion result = q.normalized();

    REQUIRE(result.length() == Approx(1.0f));
}

TEST_CASE("Quaternion normalize transforms to unit quaternion")
{
    Quaternion q{2.0f, 2.0f, 2.0f, 2.0f};

    q.normalize();

    REQUIRE(q.length() == Approx(1.0f));
}

TEST_CASE("Quaternion cannot normalize zero quaternion")
{
    Quaternion q{0.0f, 0.0f, 0.0f, 0.0f};

    REQUIRE_THROWS(q.normalized());
}

TEST_CASE("Quaternion fromAxisAngle creates Y rotation")
{
    Quaternion q = Quaternion::fromAxisAngle(Vec3{0.0f, 1.0f, 0.0f}, 90.0f);

    REQUIRE(q.w == Approx(0.70710678f));
    REQUIRE(q.x == Approx(0.0f));
    REQUIRE(q.y == Approx(0.70710678f));
    REQUIRE(q.z == Approx(0.0f));
}

TEST_CASE("Quaternion fromAxis Angle with zero angle creates identity")
{
    Quaternion q = Quaternion::fromAxisAngle(Vec3{1.0f, 0.0f, 0.0f}, 0.0f);

    REQUIRE(q.w == Approx(1.0f));
    REQUIRE(q.x == Approx(0.0f));
    REQUIRE(q.y == Approx(0.0f));
    REQUIRE(q.z == Approx(0.0f));
}

TEST_CASE("Multiplying quaternion by identity from right does not change it")
{
    Quaternion q {
        0.5f,
        0.5f,
        0.5f,
        0.5f
    };

    Quaternion identity = Quaternion::identity();

    Quaternion result = q * identity;

    REQUIRE(result.w == Approx(q.w));
    REQUIRE(result.x == Approx(q.x));
    REQUIRE(result.y == Approx(q.y));
    REQUIRE(result.z == Approx(q.z));

}

TEST_CASE("Multiplying quaternion by identity from left does not change it")
{
    Quaternion q {
        0.5f,
        0.5f,
        0.5f,
        0.5f
    };

    Quaternion identity = Quaternion::identity();

    Quaternion result = identity * q;

    REQUIRE(result.w == Approx(q.w));
    REQUIRE(result.x == Approx(q.x));
    REQUIRE(result.y == Approx(q.y));
    REQUIRE(result.z == Approx(q.z));
    
}

TEST_CASE("Two 45 degree Y rotations combine into a 90 degree Y rotation")
{
    Quaternion a = Quaternion::fromAxisAngle(Vec3{0, 1, 0}, 45.0f);
    Quaternion b = Quaternion::fromAxisAngle(Vec3{0, 1, 0}, 45.0f);

    Quaternion combined = a * b;

    Quaternion expected = Quaternion::fromAxisAngle(Vec3{0, 1, 0}, 90.0f);

    REQUIRE(combined.w == Approx(expected.w));
    REQUIRE(combined.x == Approx(expected.x));
    REQUIRE(combined.y == Approx(expected.y));
    REQUIRE(combined.z == Approx(expected.z));
}

TEST_CASE("Quaternion conjugate negates vector components")
{
    Quaternion q{1.0f, 2.0f, 3.0f, 4.0f};

    Quaternion result = q.conjugate();

    REQUIRE(result.w == Approx(1.0f));
    REQUIRE(result.x == Approx(-2.0f));
    REQUIRE(result.y == Approx(-3.0f));
    REQUIRE(result.z == Approx(-4.0f));
}

TEST_CASE("Normalized quaternion multiplied by conjugate produces identity")
{
    Quaternion q = Quaternion::fromAxisAngle(Vec3{0.0f, 1.0f, 0.0f}, 45.0f);

    Quaternion result = q * q.conjugate();

    REQUIRE(result.w == Approx(1.0f));
    REQUIRE(result.x == Approx(0.0f).margin(0.00001f));
    REQUIRE(result.y == Approx(0.0f).margin(0.00001f));
    REQUIRE(result.z == Approx(0.0f).margin(0.00001f));

}

TEST_CASE("Quaternion rotates forward 90 degrees around Y")
{
    Quaternion q = Quaternion::fromAxisAngle(Vec3{0.0f, 1.0f, 0.0f}, 90.0f);
    Vec3 forward{0.0f, 0.0f, -1.0f};

    Vec3 result = q.rotate(forward);

    REQUIRE(result.x == Approx(-1.0f).margin(0.00001f));
    REQUIRE(result.y == Approx(0.0f).margin(0.00001f));
    REQUIRE(result.z == Approx(0.0f).margin(0.00001f));
}

TEST_CASE("Identity quaternion does not rotate vector")
{
    Quaternion q = Quaternion::identity();
    Vec3 vector{2.0f, 3.0f, 4.0f};

    Vec3 result = q.rotate(vector);

    REQUIRE(result.x == Approx(2.0f));
    REQUIRE(result.y == Approx(3.0f));
    REQUIRE(result.z == Approx(4.0f));
}

