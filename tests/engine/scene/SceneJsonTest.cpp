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

    //first.components = SerializedValue::Object{};
    //second.components = SerializedValue::Object{};

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
        {"Test", SerializedValue::Array{std::string{"Test"}, 42.0f, 100.0f, false}}
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

TEST_CASE("SceneJson serializes entity components into accepted JSON")
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
        {"Test", SerializedValue::Array{std::string{"Test"}, 42.0f, 100.0f, false}}
    };

    original.entities.push_back(first);

    const std::string jsonString = SceneJson::serialize(original);

    const json result = json::parse(jsonString);

    REQUIRE(result["entities"].is_array());
    REQUIRE(result["entities"].size() == 1);

    const auto& entity = result["entities"][0];

    REQUIRE(entity["id"] == 42);
    REQUIRE(entity["components"].is_object());

    REQUIRE(entity["components"].size() == 4);
    REQUIRE(entity["components"]["Health"].is_object());

    const auto& healthComponent = entity["components"]["Health"];

    REQUIRE(healthComponent["Current Health"].is_number());
    REQUIRE(healthComponent["Current Health"] == 42);
    REQUIRE(healthComponent["Maximum Health"].is_number());
    REQUIRE(healthComponent["Maximum Health"] == 100);
    REQUIRE(healthComponent["Alive"].is_boolean());
    REQUIRE(healthComponent["Alive"]);

    REQUIRE(entity["components"]["Position"].is_object());
    const auto& positionComponent = entity["components"]["Position"];

    REQUIRE(positionComponent["x"].is_number());
    REQUIRE(positionComponent["x"] == 1.0f);
    REQUIRE(positionComponent["y"].is_number());
    REQUIRE(positionComponent["y"] == 2.0f);
    REQUIRE(positionComponent["z"].is_number());
    REQUIRE(positionComponent["z"] == 3.0f);

    REQUIRE(entity["components"]["Scale"].is_object());
    const auto& scaleComponent = entity["components"]["Scale"];

    REQUIRE(scaleComponent["x"].is_number());
    REQUIRE(scaleComponent["x"] == 4.0f);
    REQUIRE(scaleComponent["y"].is_number());
    REQUIRE(scaleComponent["y"] == 5.0f);
    REQUIRE(scaleComponent["z"].is_number());
    REQUIRE(scaleComponent["z"] == 6.0f);

    REQUIRE(entity["components"]["Test"].is_array());
    const auto& testComponent = entity["components"]["Test"];

    REQUIRE(testComponent[0].is_string());
    REQUIRE(testComponent[1].is_number());
    REQUIRE(testComponent[2].is_number());
    REQUIRE(testComponent[3].is_boolean());
}

TEST_CASE("SceneJson parses JSON back into SceneData")
{
    const std::string jsonString = R"(
    {
        "entities": [
            {
                "id": 42,
                "components": {
                    "Name": {
                        "name" : "Player"
                    },
                    "Health": {
                        "Current Health": 42.0,
                        "Alive": true
                    },
                    "Transform": {
                        "Position": {
                            "x": 1.0,
                            "y": 2.0,
                            "z": 3.0
                        },
                        "Scale": {
                            "x": 4.0,
                            "y": 5.0,
                            "z": 6.0
                        },
                        "Rotation": {
                            "w": 7.0,
                            "x": 8.0,
                            "y": 9.0,
                            "z": 10.0
                        }
                    },
                    "Test": [
                        "Test",
                        1.0,
                        2.0,
                        true
                    ]
                }
            }
        ]
    }
    )";

    SceneData sceneData = SceneJson::deserialize(jsonString);

    REQUIRE(sceneData.entities.size() == 1);
    REQUIRE(sceneData.entities[0].id == 42);

    SceneEntityData entityData = sceneData.entities[0];
    REQUIRE(entityData.components.is<SerializedValue::Object>());
    
    SerializedValue::Object componentRoot = entityData.components.get<SerializedValue::Object>();

    // Name
    SerializedValue nameComponent = componentRoot.at("Name");
    REQUIRE(nameComponent.is<SerializedValue::Object>());

    std::string nameComponentValue = nameComponent.get<SerializedValue::Object>().at("name").get<std::string>();
    REQUIRE(nameComponentValue == std::string{"Player"});

    // Health
    SerializedValue healthComponent = componentRoot.at("Health");
    REQUIRE(healthComponent.is<SerializedValue::Object>());

    float healthComponentCurrent = static_cast<float>(
        healthComponent.get<SerializedValue::Object>().at("Current Health").
        get<double>());
    REQUIRE(healthComponentCurrent == 42.0f);

    bool healthComponentAlive = 
    healthComponent.get<SerializedValue::Object>().at("Alive").
    get<bool>();
    REQUIRE(healthComponentAlive);

    // Position
    SerializedValue transformComponent = componentRoot.at("Transform");
    REQUIRE(transformComponent.is<SerializedValue::Object>());
    
    SerializedValue positionComponent = transformComponent.get<SerializedValue::Object>().at("Position");
    REQUIRE(positionComponent.is<SerializedValue::Object>());

    float positionX = static_cast<float>(
        positionComponent.get<SerializedValue::Object>().at("x").
        get<double>());
    REQUIRE(positionX == 1.0f);

    float positionY = static_cast<float>(
        positionComponent.get<SerializedValue::Object>().at("y").
        get<double>());
    REQUIRE(positionY == 2.0f);

    float positionZ = static_cast<float>(
        positionComponent.get<SerializedValue::Object>().at("z").
        get<double>());
    REQUIRE(positionZ == 3.0f);

    // Scale
    SerializedValue scaleComponent = transformComponent.get<SerializedValue::Object>().at("Scale");
    REQUIRE(scaleComponent.is<SerializedValue::Object>());

    float scaleX = static_cast<float>(
        scaleComponent.get<SerializedValue::Object>().at("x").
        get<double>());
    REQUIRE(scaleX == 4.0f);

    float scaleY = static_cast<float>(
        scaleComponent.get<SerializedValue::Object>().at("y").
        get<double>());
    REQUIRE(scaleY == 5.0f);

    float scaleZ = static_cast<float>(
        scaleComponent.get<SerializedValue::Object>().at("z").
        get<double>());
    REQUIRE(scaleZ == 6.0f);

    // Rotation
    SerializedValue rotationComponent = transformComponent.get<SerializedValue::Object>().at("Rotation");
    REQUIRE(rotationComponent.is<SerializedValue::Object>());

    float rotationW = static_cast<float>(
        rotationComponent.get<SerializedValue::Object>().at("w").
        get<double>());
    REQUIRE(rotationW == 7.0f);

    float rotationX = static_cast<float>(
        rotationComponent.get<SerializedValue::Object>().at("x").
        get<double>());
    REQUIRE(rotationX == 8.0f);

    float rotationY = static_cast<float>(
        rotationComponent.get<SerializedValue::Object>().at("y").
        get<double>());
    REQUIRE(rotationY == 9.0f);

    float rotationZ = static_cast<float>(
        rotationComponent.get<SerializedValue::Object>().at("z").
        get<double>());
    REQUIRE(rotationZ == 10.0f);

    
    // Test 
    SerializedValue testComponent = componentRoot.at("Test");
    REQUIRE(testComponent.is<SerializedValue::Array>());

    std::string testValueA = testComponent.get<SerializedValue::Array>().at(0).get<std::string>();
    REQUIRE(testValueA == std::string{"Test"});

    float testValueB = static_cast<float>(testComponent.get<SerializedValue::Array>().at(1).get<double>());
    REQUIRE(testValueB == 1.0f);

    float testValueC = static_cast<float>(testComponent.get<SerializedValue::Array>().at(2).get<double>());
    REQUIRE(testValueC == 2.0f);

    float testValueD = testComponent.get<SerializedValue::Array>().at(3).get<bool>();
    REQUIRE(testValueD);

}

TEST_CASE("SceneJson preserves large integer IDs")
{
    SceneData original;
    SceneEntityData first;
    first.id = 18014398509481985;
    first.components = SerializedValue::Object{
    };

    original.entities.push_back(first);

    const std::string json = SceneJson::serialize(original);
    const SceneData result = SceneJson::deserialize(json);

    REQUIRE(result.entities.size() == 1);
    REQUIRE(result.entities[0].id == 18014398509481985);

}

TEST_CASE("SceneJson serializes empty components")
{
    SceneData original;
    SceneEntityData first;
    first.id = 42;
    first.components = SerializedValue::Object{
    };

    original.entities.push_back(first);

    const std::string json = SceneJson::serialize(original);
    const SceneData result = SceneJson::deserialize(json);

    REQUIRE(result.entities.size() == 1);
    REQUIRE(result.entities[0].id == 42);

    REQUIRE(result.entities[0].components.is<SerializedValue::Object>());
    SerializedValue::Object components = result.entities[0].components.get<SerializedValue::Object>();

    REQUIRE(components.size() == 0);
}

TEST_CASE("SceneJson correctly parses negative integers")
{
    SceneData original;
    SceneEntityData first;
    first.id = 42;
    first.components = SerializedValue::Object{
        {"Negative Component", SerializedValue::Object{
           {"negative", SerializedValue{std::int64_t{-42}}}
        }}
    };

    original.entities.push_back(first);

    const std::string json = SceneJson::serialize(original);
    const SceneData result = SceneJson::deserialize(json);

    REQUIRE(result.entities.size() == 1);
    REQUIRE(result.entities[0].id == 42);

    REQUIRE(result.entities[0].components.is<SerializedValue::Object>());
    SerializedValue::Object componentRoot = result.entities[0].components.get<SerializedValue::Object>();

    REQUIRE(componentRoot.at("Negative Component").is<SerializedValue::Object>());
    SerializedValue::Object negativeComponent = componentRoot.at("Negative Component").get<SerializedValue::Object>();

    REQUIRE(negativeComponent.at("negative").get<int64_t>() == -42);
}

TEST_CASE("SceneJson serializes null components")
{
    SceneData original;
    SceneEntityData first;
    first.id = 42;

    original.entities.push_back(first);

    std::string jsonString = SceneJson::serialize(original);
    SceneData result = SceneJson::deserialize(jsonString);

    REQUIRE(result.entities[0].components.is<std::nullptr_t>());
}

TEST_CASE("SceneJson serializes empty object")
{
    SceneData original;
    SceneEntityData first;
    first.id = 42;
    first.components = SerializedValue::Object{};

    original.entities.push_back(first);

    std::string jsonString = SceneJson::serialize(original);
    SceneData result = SceneJson::deserialize(jsonString);

    REQUIRE(result.entities[0].components.is<SerializedValue::Object>());
    REQUIRE(result.entities[0].components.get<SerializedValue::Object>().size() == 0);
}

TEST_CASE("SceneJson serializes empty array")
{
    SceneData original;
    SceneEntityData first;
    first.id = 42;
    first.components = SerializedValue::Array{};

    original.entities.push_back(first);

    std::string jsonString = SceneJson::serialize(original);
    SceneData result = SceneJson::deserialize(jsonString);

    REQUIRE(result.entities[0].components.is<SerializedValue::Array>());
    REQUIRE(result.entities[0].components.get<SerializedValue::Array>().size() == 0);
}


