#include <catch2/catch_test_macros.hpp>

#include "engine/serialization/SerializedValue.h"

TEST_CASE("SerializedValue stores numbers")
{
    SerializedValue value{42.0};
    REQUIRE(value.is<double>());
    REQUIRE(value.get<double>() == 42.0);
}

TEST_CASE("SerializedValue stores objects")
{
    SerializedValue::Object object;

    object["health"] = SerializedValue{75.0};
    SerializedValue value{std::move(object)};

    REQUIRE(value.is<SerializedValue::Object>());

    const auto& storedObject = value.get<SerializedValue::Object>();

    REQUIRE(storedObject.at("health").get<double>() == 75.0);
}