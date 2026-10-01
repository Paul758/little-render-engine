#include <catch2/catch_test_macros.hpp>

#include "engine/core/ecs/EntityManager.h"

TEST_CASE("EntityManager creates a new Entity")
{
    EntityManager entityManager;

    Entity created1 = entityManager.create();
    Entity created2 = entityManager.create();
    Entity created3 = entityManager.create();

    CHECK(created1.index == 0);
    CHECK(created1.generation == 0);

    CHECK(created2.index == 1);
    CHECK(created2.generation == 0);

    CHECK(created3.index == 2);
    CHECK(created3.generation == 0);
}

TEST_CASE("EntityManager destroys entities")
{
    EntityManager entityManager;

    Entity created1 = entityManager.create();
    Entity created2 = entityManager.create();
    Entity created3 = entityManager.create(); 

    entityManager.destroy(created1);

    CHECK(entityManager.isAlive(created1) == false);
    
    Entity secondGeneration1 = entityManager.create();
    CHECK(secondGeneration1.generation == 1);
}

TEST_CASE("EntityManager recognizes alive entities")
{
    EntityManager entityManager;

    Entity created1 = entityManager.create();

    CHECK(entityManager.isAlive(created1) == true);
}

TEST_CASE("EntityManager contains alive entities")
{
    EntityManager entityManager;

    Entity a = entityManager.create();
    Entity b = entityManager.create();
    Entity c = entityManager.create();

    const auto& alive = entityManager.getAliveEntities();

    REQUIRE(alive.size() == 3);
    REQUIRE(std::find(alive.begin(), alive.end(), a) != alive.end());
    REQUIRE(std::find(alive.begin(), alive.end(), b) != alive.end());
    REQUIRE(std::find(alive.begin(), alive.end(), c) != alive.end());

}

TEST_CASE("EntityManager removes destroyed entity from alive entities")
{
    EntityManager entityManager;

    Entity a = entityManager.create();
    Entity b = entityManager.create();
    Entity c = entityManager.create();

    entityManager.destroy(b);

    const auto& alive = entityManager.getAliveEntities();

    REQUIRE(alive.size() == 2);
    REQUIRE(std::find(alive.begin(), alive.end(), a) != alive.end());
    REQUIRE(std::find(alive.begin(), alive.end(), b) == alive.end());
    REQUIRE(std::find(alive.begin(), alive.end(), c) != alive.end());

}

TEST_CASE("EntityManager lookup remains correct after swap removal")
{
    EntityManager entityManager;

    Entity a = entityManager.create();
    Entity b = entityManager.create();
    Entity c = entityManager.create();

    entityManager.destroy(b);
    entityManager.destroy(c);

    const auto& alive = entityManager.getAliveEntities();

    REQUIRE(alive.size() == 1);
    REQUIRE(alive[0] == a);

    REQUIRE(entityManager.isAlive(a));
    REQUIRE_FALSE(entityManager.isAlive(b));
    REQUIRE_FALSE(entityManager.isAlive(c));
}

TEST_CASE("EntityManager reuses destroyed entity index with new generation")
{
    EntityManager entityManager;

    Entity oldEntity = entityManager.create();
    entityManager.destroy(oldEntity);

    Entity newEntity = entityManager.create();

    REQUIRE(newEntity.index == oldEntity.index);
    REQUIRE(newEntity.generation != oldEntity.generation);

    REQUIRE_FALSE(entityManager.isAlive(oldEntity));
    REQUIRE(entityManager.isAlive(newEntity));

    const auto& alive = entityManager.getAliveEntities();

    REQUIRE(alive.size() == 1);
    REQUIRE(alive[0] == newEntity);
}

TEST_CASE("EntityManager reamins correct after destruction and index reuse")
{
    EntityManager entityManager;

    Entity a = entityManager.create();
    Entity b = entityManager.create();
    Entity c = entityManager.create();

    entityManager.destroy(b);

    Entity d = entityManager.create();

    REQUIRE(d.index == b.index);
    REQUIRE(d.generation != b.generation);

    entityManager.destroy(c);

    const auto& alive = entityManager.getAliveEntities();
    
    REQUIRE(alive.size() == 2);
    REQUIRE(std::find(alive.begin(), alive.end(), a) != alive.end());
    REQUIRE(std::find(alive.begin(), alive.end(), d) != alive.end());

    REQUIRE(entityManager.isAlive(a));
    REQUIRE(entityManager.isAlive(d));

    REQUIRE_FALSE(entityManager.isAlive(b));
    REQUIRE_FALSE(entityManager.isAlive(c));
}