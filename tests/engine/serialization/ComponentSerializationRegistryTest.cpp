#include <catch2/catch_test_macros.hpp>

#include "engine/serialization/ComponentSerializationRegistry.h"
#include "engine/core/ecs/World.h"
#include "engine/serialization/EngineSerialization.h"
#include "engine/components/NameComponent.h"
#include "engine/components/TransformComponent.h"
#include "engine/math/Vec3.h"

namespace
{
    struct PositionComponent
    {
        float x = 0.0f;
    };

    struct HealthComponent
    {
        float health = 100.0f;
    };
}

TEST_CASE("ComponentSerializationRegistry returns registered components present on entity")
{
    World world;
    ComponentSerializationRegistry registry;

    registry.registerComponent<PositionComponent>(
        "Position",
        [](const PositionComponent& position)
        {
            SerializedValue::Object object;
            object["x"] = SerializedValue{static_cast<double>(position.x)};
            return SerializedValue{std::move(object)};
        },
        [](const SerializedValue& value)
        {
            const auto& object = value.get<SerializedValue::Object>();
            PositionComponent position;
            position.x = static_cast<float>(object.at("x").get<double>());
            return position;
        }
    );

    Entity entity = world.createEntity();

    world.components().add(entity, PositionComponent{42.0f});

    const SerializedValue serialized = registry.serializeComponents(world, entity);
    REQUIRE(serialized.is<SerializedValue::Object>());

    const auto& components = serialized.get<SerializedValue::Object>();
    REQUIRE(components.contains("Position"));

    const SerializedValue& positionValue = components.at("Position");

    REQUIRE(positionValue.is<SerializedValue::Object>());

    const auto& position = positionValue.get<SerializedValue::Object>();
    REQUIRE(position.at("x").get<double>() == 42.0);

}

TEST_CASE("ComponentSerializationRegistry round trips a component")
{
        World world;
    ComponentSerializationRegistry registry;

    registry.registerComponent<PositionComponent>(
        "Position",
        [](const PositionComponent& position)
        {
            SerializedValue::Object object;
            object["x"] = SerializedValue{static_cast<double>(position.x)};
            return SerializedValue{std::move(object)};
        },
        [](const SerializedValue& value)
        {
            const auto& object = value.get<SerializedValue::Object>();
            PositionComponent position;
            position.x = static_cast<float>(object.at("x").get<double>());
            return position;
        }
    );

    Entity source = world.createEntity();

    world.components().add(source, PositionComponent{42.0f});

    const SerializedValue serialized = registry.serializeComponents(world, source);

    Entity destination = world.createEntity();

    REQUIRE_FALSE(world.components().has<PositionComponent>(destination));

    registry.deserializeComponents(world, destination, serialized);

    const PositionComponent* position = world.components().get<PositionComponent>(destination);

    REQUIRE(position != nullptr);
    REQUIRE(position->x == 42.0f);
}

TEST_CASE("Engine serialization round trips NameComponent")
{
    World world;
    ComponentSerializationRegistry registry;

    EngineSerialization::registerComponents(registry);

    Entity source = world.createEntity();

    world.components().add(source, NameComponent{"Player"});

    const SerializedValue serialized = registry.serializeComponents(world, source);

    Entity destination = world.createEntity();

    registry.deserializeComponents(world, destination, serialized);

    const NameComponent* name = world.components().get<NameComponent>(destination);

    REQUIRE(name != nullptr);
    REQUIRE(name->name == "Player");
}

TEST_CASE("Engine serialization round trips TransformComponent")
{
    World world;
    ComponentSerializationRegistry registry;

    EngineSerialization::registerComponents(registry);

    Entity source = world.createEntity();

    TransformComponent transform;
    transform.position = Vec3{1.0f, 2.0f, 3.0f};
    transform.rotation = Quaternion::fromEuler(Vec3{10.0f, 20.0f, 30.0f});
    transform.scale = Vec3{2.0f, 2.0f, 2.0f};

    world.components().add(source, transform);

    const SerializedValue serialized = registry.serializeComponents(world, source);

    Entity destination = world.createEntity();

    REQUIRE_FALSE(world.components().has<TransformComponent>(destination));

    registry.deserializeComponents(world, destination, serialized);

    const TransformComponent* transformResult = world.components().get<TransformComponent>(destination);

    REQUIRE(transformResult != nullptr);
    REQUIRE(transformResult->position.x == transform.position.x);
    REQUIRE(transformResult->position.y == transform.position.y);
    REQUIRE(transformResult->position.z == transform.position.z);

    REQUIRE(transformResult->scale.x == transform.scale.x);
    REQUIRE(transformResult->scale.y == transform.scale.y);
    REQUIRE(transformResult->scale.z == transform.scale.z);

    REQUIRE(transformResult->rotation.w == transform.rotation.w);
    REQUIRE(transformResult->rotation.x == transform.rotation.x);
    REQUIRE(transformResult->rotation.y == transform.rotation.y);
    REQUIRE(transformResult->rotation.z == transform.rotation.z);
}


