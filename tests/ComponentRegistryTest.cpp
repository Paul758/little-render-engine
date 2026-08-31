#include <catch2/catch_test_macros.hpp>

#include "ecs/ComponentRegistry.h"
#include "components/TransformComponent.h"
#include "components/VelocityComponent.h"

TEST_CASE("ComponentRegistry adds and stores a component")
{
    ComponentRegistry componentRegistry;

    Entity entity;

    TransformComponent transform;
    transform.position = {1.0f, 2.0f, 3.0f};

    TransformComponent& storedTransform = componentRegistry.add(entity, transform);

    CHECK(storedTransform.position.x == 1.0f);
    CHECK(storedTransform.position.y == 2.0f);
    CHECK(storedTransform.position.z == 3.0f);

    TransformComponent* retrievedTransform = componentRegistry.get<TransformComponent>(entity);

    REQUIRE(retrievedTransform != nullptr);

    CHECK(&storedTransform == retrievedTransform);

    CHECK(retrievedTransform->position.x == 1.0f);
    CHECK(retrievedTransform->position.y == 2.0f);
    CHECK(retrievedTransform->position.z == 3.0f);
}

TEST_CASE("Check if an entity has a component")
{
    ComponentRegistry componentRegistry;

    Entity entity;

    TransformComponent transform;
    transform.position = {1.0f, 2.0f, 3.0f};

    TransformComponent& storedTransform = componentRegistry.add(entity, transform);

    bool has = componentRegistry.has<TransformComponent>(entity);

    CHECK(has == true);
}

TEST_CASE("ComponentRegistry removes an Entity from all pools")
{
    ComponentRegistry componentRegistry;

    Entity entity;

    componentRegistry.add(entity, TransformComponent{});
    componentRegistry.add(entity, VelocityComponent{});

    REQUIRE(componentRegistry.has<TransformComponent>(entity) == true);
    REQUIRE(componentRegistry.has<VelocityComponent>(entity) == true);

    componentRegistry.removeAll(entity);

    CHECK(componentRegistry.has<TransformComponent>(entity) == false);
    CHECK(componentRegistry.has<VelocityComponent>(entity) == false);
}