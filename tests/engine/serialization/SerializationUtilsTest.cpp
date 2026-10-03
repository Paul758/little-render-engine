#include <catch2/catch_test_macros.hpp>

#include "engine/serialization/SerializationUtils.h"

TEST_CASE("Vec3 serialization round trips")
{
    const Vec3 original {1.5f, -2.0f, 7.25f};

    const SerializedValue serialized = SerializationUtils::serializeVec3(original);

    const Vec3 result = SerializationUtils::deserializeVec3(serialized);

    REQUIRE(original.x == result.x);
    REQUIRE(original.y == result.y);
    REQUIRE(original.z == result.z);
}

TEST_CASE("Quaternion serialization round trips")
{
    const Quaternion original = Quaternion::fromEuler(Vec3{10.0f, 25.0f, 40.0f});

    const SerializedValue serialized = SerializationUtils::serializeQuaternion(original);

    const Quaternion result = SerializationUtils::deserializeQuaternion(serialized);

    REQUIRE(original.w == result.w);
    REQUIRE(original.x == result.x);
    REQUIRE(original.y == result.y);
    REQUIRE(original.z == result.z);
}