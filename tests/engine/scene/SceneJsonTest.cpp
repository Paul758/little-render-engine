#include <catch2/catch_test_macros.hpp>

#include "engine/scene/SceneData.h"
#include "engine/scene/SceneJson.h"
#include <string>

TEST_CASE("SceneJson round trips empty scene")
{
    SceneData original;

    const std::string json = SceneJson::serialize(original);
    const SceneData result = SceneJson::deserialize(json);

    REQUIRE(result.entities.empty());
}

TEST_CASE("SceneJson round trips scene entity ids")
{
    SceneData original;

    SceneEntityData first;
    first.id = 42;

    SceneEntityData second;
    second.id = 100;

    original.entities.push_back(first);
    original.entities.push_back(second);

    const std::string json = SceneJson::serialize(original);
    const SceneData result = SceneJson::deserialize(json);

    REQUIRE(result.entities.size() == 2);
    REQUIRE(result.entities[0].id == 42);
    REQUIRE(result.entities[1].id == 100);
}

TEST_CASE("SceneJson serializes entity components")
{
    SceneData original;
    SceneEntityData first;
    first.id = 42;
    first.components = SerializedValue::Object{
        {"Health", SerializedValue::Object{
            {"Current Health", SerializedValue{42.0f}},
            {"Maximum Health", SerializedValue{100.0f}},
            {"Alive", SerializedValue{true}},
        }},
        {"Position", SerializedValue::Object{
            {"x", SerializedValue{1.0f}},
            {"y", SerializedValue{2.0f}},
            {"z", SerializedValue{3.0f}},
        }},
        {"Scale", SerializedValue::Object{
                {"x", SerializedValue{4.0f}},
                {"y", SerializedValue{5.0f}},
                {"z", SerializedValue{6.0f}},
        }},
        {"Test", SerializedValue::Array{"Test", 42.0f, 100.0f, false}}
    };

    original.entities.push_back(first);

    const std::string json = SceneJson::serialize(original);
    const SceneData result = SceneJson::deserialize(json);

    REQUIRE(result.entities.size() == 1);
    REQUIRE(result.entities[0].components.is<SerializedValue::Object>());
    SerializedValue::Object root = result.entities[0].components.get<SerializedValue::Object>();

    REQUIRE(root.contains("Health"));
    REQUIRE(root.contains("Position"));
    REQUIRE(root.contains("Scale"));
    REQUIRE(root.contains("Test"));

    SerializedValue::Object healthComponent = root["Health"].get<SerializedValue::Object>();
    REQUIRE(healthComponent.contains("Current Health"));
    REQUIRE(healthComponent.contains("Maximum Health"));
    REQUIRE(healthComponent.contains("Alive"));

    SerializedValue current = healthComponent.at("Current Health");
    SerializedValue max = healthComponent.at("Maximum Health");
    SerializedValue alive = healthComponent.at("Alive");
    REQUIRE(current.is<double>());
    REQUIRE(max.is<double>());
    REQUIRE(alive.is<bool>());

    REQUIRE(static_cast<float>(current.get<double>()) == 42.0f);
    REQUIRE(static_cast<float>(max.get<double>()) == 100.0f);
    REQUIRE(alive.get<bool>());

    // Position
    SerializedValue::Object position = root["Position"].get<SerializedValue::Object>();
    REQUIRE(position.contains("x"));
    REQUIRE(position.contains("y"));
    REQUIRE(position.contains("z"));

    SerializedValue x = position.at("x");
    SerializedValue y = position.at("y");
    SerializedValue z = position.at("z");
    REQUIRE(x.is<double>());
    REQUIRE(y.is<double>());
    REQUIRE(z.is<double>());

    REQUIRE(static_cast<float>(x.get<double>()) == 1.0f);
    REQUIRE(static_cast<float>(y.get<double>()) == 2.0f);
    REQUIRE(static_cast<float>(z.get<double>()) == 3.0f);

    // Scale
    SerializedValue::Object scale = root["Scale"].get<SerializedValue::Object>();
    REQUIRE(scale.contains("x"));
    REQUIRE(scale.contains("y"));
    REQUIRE(scale.contains("z"));

    SerializedValue scaleX = scale.at("x");
    SerializedValue scaleY = scale.at("y");
    SerializedValue scaleZ = scale.at("z");
    REQUIRE(scaleX.is<double>());
    REQUIRE(scaleY.is<double>());
    REQUIRE(scaleZ.is<double>());

    REQUIRE(static_cast<float>(scaleX.get<double>()) == 4.0f);
    REQUIRE(static_cast<float>(scaleY.get<double>()) == 5.0f);
    REQUIRE(static_cast<float>(scaleZ.get<double>()) == 6.0f);

    //Test
    SerializedValue::Array test = root["Test"].get<SerializedValue::Array>();

    SerializedValue a = test[0];
    SerializedValue b = test[1];
    SerializedValue c = test[2];
    SerializedValue d = test[3];

    REQUIRE(a.is<std::string>());
    REQUIRE(b.is<double>());
    REQUIRE(c.is<double>());
    REQUIRE(d.is<bool>());

    REQUIRE((a.get<std::string>()) == "Test");
    REQUIRE(static_cast<float>(b.get<double>()) == 42.0f);
    REQUIRE(static_cast<float>(c.get<double>()) == 100.0f);
    REQUIRE(d.get<bool>() == false);
}