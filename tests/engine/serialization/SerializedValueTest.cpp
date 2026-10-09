#include <catch2/catch_test_macros.hpp>

#include "engine/serialization/SerializedValue.h"

TEST_CASE("SerializedValue stores floating point numbers")
{
    SerializedValue value{42.0};
    REQUIRE(value.is<double>());
    REQUIRE(value.get<double>() == 42.0);
}

TEST_CASE("SerializedValue stores unsigned integer numbers")
{
    SerializedValue value{std::uint64_t{42}};
    REQUIRE(value.is<std::uint64_t>());
    REQUIRE(value.get<std::uint64_t>() == 42);
}

TEST_CASE("SerializedValue stores signed integer numbers")
{
    SerializedValue value{std::int64_t{-42}};
    REQUIRE(value.is<std::int64_t>());
    REQUIRE(value.get<std::int64_t>() == -42);
}

TEST_CASE("SerializedValue stores boolean values")
{
    SerializedValue value{true};
    REQUIRE(value.is<bool>());
    REQUIRE(value.get<bool>());
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

TEST_CASE("SerializedValue stores arrays")
{
    SerializedValue::Array array;

    array.push_back(SerializedValue{75.0});
    SerializedValue value{std::move(array)};

    REQUIRE(value.is<SerializedValue::Array>());

    const auto& storedArray = value.get<SerializedValue::Array>();

    REQUIRE(storedArray.at(0).get<double>() == 75.0);
}

TEST_CASE("SerializedValue defaults to null")
{
    SerializedValue value;
    REQUIRE(value.is<std::nullptr_t>());
}

TEST_CASE("SerializeValue supports explicit null")
{
    SerializedValue value{nullptr};
    REQUIRE(value.is<std::nullptr_t>());
}

TEST_CASE("SerializedValue supports empty object")
{
    SerializedValue value {SerializedValue::Object{}};

    REQUIRE(value.is<SerializedValue::Object>());
    REQUIRE(value.get<SerializedValue::Object>().size() == 0);

}

TEST_CASE("SerializedValue supports empty array")
{
    SerializedValue value {SerializedValue::Array{}};

    REQUIRE(value.is<SerializedValue::Array>());
    REQUIRE(value.get<SerializedValue::Array>().size() == 0); 
}

TEST_CASE("SerializedValue throws at bad variant access")
{
    SerializedValue value{42.0f};
    REQUIRE_THROWS_AS(value.get<std::string>(), std::bad_variant_access);
}