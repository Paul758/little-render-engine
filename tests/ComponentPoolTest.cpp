#include <catch2/catch_test_macros.hpp>

#include "ecs/ComponentPool.h"
#include "components/TransformComponent.h"

TEST_CASE("ComponentPool stores and retrieves a component")
{
    ComponentPool<TransformComponent> pool;
    Entity entity;

    TransformComponent transform;
    transform.position = {1.0f, 2.0f, 3.0f};

    pool.add(entity, transform);

    TransformComponent* result = pool.get(entity);

    REQUIRE(result != nullptr);

    CHECK(result->position.x == 1.0f);
    CHECK(result->position.y == 2.0f);
    CHECK(result->position.z == 3.0f);
}

TEST_CASE("ComponentPool returns nullptr for missing entity")
{
    ComponentPool<TransformComponent> pool;
    Entity entity{42, 0};
    TransformComponent* result = pool.get(entity);

    CHECK(result == nullptr);
}

TEST_CASE("ComponentPool reports whether an entity has a component")
{
    ComponentPool<TransformComponent> pool;

    Entity entity;

    CHECK_FALSE(pool.has(entity));

    pool.add(entity, TransformComponent{});

    CHECK(pool.has(entity));
}

TEST_CASE("ComponentPool removes a component")
{
    ComponentPool<TransformComponent> pool;

    Entity entity;

    pool.add(entity, TransformComponent{});

    REQUIRE(pool.has(entity));

    pool.remove(entity);

    CHECK_FALSE(pool.has(entity));
    CHECK(pool.get(entity) == nullptr);
}

TEST_CASE("Removing one component keeps other entity mappings valid")
{
    ComponentPool<TransformComponent> pool;

    Entity entity1{1, 0};
    Entity entity2{2, 0};
    Entity entity3{3, 0};

    TransformComponent a;
    a.position = {1.0f, 0.0f, 0.0f};

    TransformComponent b;
    b.position = {2.0f, 0.0f, 0.0f};

    TransformComponent c;
    c.position = {3.0f, 0.0f, 0.0f};

    pool.add(entity1, a);
    pool.add(entity2, b);
    pool.add(entity3, c);

    pool.remove(entity2);

    CHECK(pool.get(entity2) == nullptr);

    REQUIRE(pool.get(entity1) != nullptr);
    REQUIRE(pool.get(entity3) != nullptr);

    CHECK(pool.get(entity1)->position.x == 1.0f);
    CHECK(pool.get(entity3)->position.x == 3.0f);
}