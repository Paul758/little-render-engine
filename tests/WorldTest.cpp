#include <catch2/catch_test_macros.hpp>

#include "ecs/World.h"
#include "components/TransformComponent.h"
#include "components/VelocityComponent.h"

TEST_CASE("World creates entities")
{
    World world;

    Entity created1 = world.createEntity();

    CHECK(world.isAlive(created1) == true);
}

TEST_CASE("World destroys entity")
{
    World world;

    Entity created1 = world.createEntity();

    CHECK(world.isAlive(created1));

    world.destroyEntity(created1);

    CHECK(world.isAlive(created1) == false);
}

TEST_CASE("World removes entity from all component pools when destroying entity")
{
    World world;

    Entity created1 = world.createEntity();
    TransformComponent transform;
    VelocityComponent velocity;

    REQUIRE(world.isAlive(created1));

    world.components().add(created1, transform);
    world.components().add(created1, velocity);

    REQUIRE(world.components().has<TransformComponent>(created1) == true);
    REQUIRE(world.components().has<VelocityComponent>(created1) == true);

    world.destroyEntity(created1);

    CHECK(world.components().has<TransformComponent>(created1) == false);
    CHECK(world.components().has<VelocityComponent>(created1) == false);

    CHECK(world.isAlive(created1) == false);
}

TEST_CASE("Reused entity index gets a new generation")
{
    World world;

    Entity oldEntity = world.createEntity();

    world.destroyEntity(oldEntity);

    Entity newEntity = world.createEntity();

    CHECK(newEntity.index == oldEntity.index);
    CHECK(newEntity.generation != oldEntity.generation);

    CHECK_FALSE(world.isAlive(oldEntity));
    CHECK(world.isAlive(newEntity));
}

TEST_CASE("Old entity handle cannot access components of a recycled entity")
{
    World world;

    Entity oldEntity = world.createEntity();

    world.components().add(oldEntity, TransformComponent{});
    world.destroyEntity(oldEntity);

    Entity newEntity = world.createEntity();

    world.components().add(newEntity, TransformComponent{});

    CHECK_FALSE(world.components().has<TransformComponent>(oldEntity));
    CHECK(world.components().has<TransformComponent>(newEntity));
}