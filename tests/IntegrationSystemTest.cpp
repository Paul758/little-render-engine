#include <catch2/catch_test_macros.hpp>

#include "systems/IntegrationSystem.h"
#include "ecs/World.h"
#include "components/TransformComponent.h"
#include "components/VelocityComponent.h"

TEST_CASE("Movement System finds Entities with Transform and Velocity")
{
    GameTime gameTime;
    World world;

    Entity entity = world.createEntity();

    TransformComponent transform;
    transform.position = {0.0f, 0.0f, 0.0f};

    VelocityComponent velocity;
    velocity.velocity = {1.0f, 1.0f, 1.0f};

    world.components().add(entity, transform);
    world.components().add(entity, velocity);

    REQUIRE(world.components().has<TransformComponent>(entity));
    REQUIRE(world.components().has<VelocityComponent>(entity));

    IntegrationSystem integrationSystem(world.components());

    integrationSystem.update(1.0f);

    TransformComponent* retrievedTransform = world.components().get<TransformComponent>(entity);

    REQUIRE(retrievedTransform != nullptr);

    CHECK(retrievedTransform->position.x == 1.0f);
    CHECK(retrievedTransform->position.y == 1.0f);
    CHECK(retrievedTransform->position.z == 1.0f);
}

TEST_CASE("Movement System ignores entities that dont contain Transform and Velocity components")
{
    GameTime gameTime;
    World world;

    Entity entityA = world.createEntity();
    Entity entityB = world.createEntity();
    Entity entityC = world.createEntity();

    TransformComponent transform;
    transform.position = {0.0f, 0.0f, 0.0f};

    VelocityComponent velocity;
    velocity.velocity = {1.0f, 1.0f, 1.0f};

    world.components().add(entityA, transform);
    world.components().add(entityA, velocity);

    world.components().add(entityB, TransformComponent{});
    world.components().add(entityC, VelocityComponent{});

    REQUIRE(world.components().has<TransformComponent>(entityB));
    REQUIRE(world.components().has<VelocityComponent>(entityC));

    REQUIRE(world.components().has<TransformComponent>(entityA));
    REQUIRE(world.components().has<VelocityComponent>(entityA));

    IntegrationSystem integrationSystem(world.components());

    integrationSystem.update(1.0f);

    TransformComponent* retrievedTransform = world.components().get<TransformComponent>(entityA);

    REQUIRE(retrievedTransform != nullptr);

    CHECK(retrievedTransform->position.x == 1.0f);
    CHECK(retrievedTransform->position.y == 1.0f);
    CHECK(retrievedTransform->position.z == 1.0f);

    TransformComponent* retrievedEntityB = world.components().get<TransformComponent>(entityB);
    VelocityComponent* retrievedEntityC = world.components().get<VelocityComponent>(entityC);
    
}