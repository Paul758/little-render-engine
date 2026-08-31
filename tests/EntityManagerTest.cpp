#include <catch2/catch_test_macros.hpp>

#include "ecs/EntityManager.h"

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