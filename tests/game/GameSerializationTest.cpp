#include <catch2/catch_test_macros.hpp>

#include "game/serialization/GameSerialization.h"
#include "game/components/HealthComponent.h"

#include "engine/core/ecs/World.h"
#include "engine/serialization/ComponentSerializationRegistry.h"
#include "engine/serialization/EngineSerialization.h"
#include "engine/serialization/SerializationContext.h"
#include "engine/serialization/DeserializationContext.h"
#include "engine/components/NameComponent.h"

TEST_CASE("Game serialization round trips HealthComponent")
{
    World world;
    ComponentSerializationRegistry registry;

    SerializationContext context;

    GameSerialization::registerComponents(registry);

    Entity source = world.createEntity();
    world.components().add(source, HealthComponent{75.0f, 100.0f});

    const SerializedValue serialized = registry.serializeComponents(world, source, context);

    Entity destination = world.createEntity();

    DeserializationContext deserializationContext;
    deserializationContext.registerEntity(1, destination);

    REQUIRE_FALSE(world.components().has<HealthComponent>(destination));

    registry.deserializeComponents(world, destination, serialized, deserializationContext);

    REQUIRE(world.components().has<HealthComponent>(destination));

    HealthComponent* health = world.components().get<HealthComponent>(destination);

    REQUIRE(health != nullptr);
    REQUIRE(health->currentHealth == 75.0f);
    REQUIRE(health->maxHealth == 100.0f);

}

TEST_CASE("Engine and game components can share serialization registry")
{
    World world;
    ComponentSerializationRegistry registry;

    SerializationContext context;

    EngineSerialization::registerComponents(registry);
    GameSerialization::registerComponents(registry);

    Entity source = world.createEntity();

    world.components().add(source, NameComponent{"Player"});
    world.components().add(source, HealthComponent{75.0f, 100.0f});

    const SerializedValue serialized = registry.serializeComponents(world, source, context);

    Entity destination = world.createEntity();

    REQUIRE_FALSE(world.components().has<NameComponent>(destination));
    REQUIRE_FALSE(world.components().has<HealthComponent>(destination));

    DeserializationContext deserializationContext;
    deserializationContext.registerEntity(1, destination);

    registry.deserializeComponents(world, destination, serialized, deserializationContext);

    REQUIRE(world.components().has<NameComponent>(destination));
    REQUIRE(world.components().has<HealthComponent>(destination));

    NameComponent* name = world.components().get<NameComponent>(destination);
    HealthComponent* health = world.components().get<HealthComponent>(destination);

    REQUIRE(name != nullptr);
    REQUIRE(name->name == "Player");

    REQUIRE(health != nullptr);
    REQUIRE(health->currentHealth == 75.0f);
    REQUIRE(health->maxHealth == 100.0f);
}