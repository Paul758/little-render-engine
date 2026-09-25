#include <catch2/catch_test_macros.hpp>

#include <algorithm>

#include "engine/systems/HierarchySystem.h"
#include "engine/core/ecs/World.h"
#include "engine/core/ecs/Entity.h"
#include "engine/components/ParentComponent.h"

TEST_CASE("HierarchySystem sets a parent")
{
    World world;
    Entity child = world.createEntity();
    Entity parent = world.createEntity();

    HierarchySystem hierarchySystem(world.components());

    hierarchySystem.setParent(child, parent);

    ParentComponent* retrievedParent = world.components().get<ParentComponent>(child);

    REQUIRE(retrievedParent != nullptr);
    REQUIRE(retrievedParent->parent == parent);
}

TEST_CASE("HierarchySystem removes a parent")
{
    World world;
    Entity child = world.createEntity();
    Entity parent = world.createEntity();

    HierarchySystem hierarchySystem(world.components());

    hierarchySystem.setParent(child, parent);

    ParentComponent* retrievedParent = world.components().get<ParentComponent>(child);

    REQUIRE(retrievedParent != nullptr);
    REQUIRE(retrievedParent->parent == parent);

    hierarchySystem.removeParent(child);

    ParentComponent* nullPointerParent = world.components().get<ParentComponent>(child);

    REQUIRE(nullPointerParent == nullptr);
}

TEST_CASE("HierarchySystem recognizes that child has a parent")
{
    World world;
    Entity child = world.createEntity();
    Entity parent = world.createEntity();

    HierarchySystem hierarchySystem(world.components());

    REQUIRE_FALSE(hierarchySystem.hasParent(child));

    hierarchySystem.setParent(child, parent);

    REQUIRE(hierarchySystem.hasParent(child));
}

TEST_CASE("HierarchySystem returns parent for a child that has a parent")
{
    World world;
    Entity child = world.createEntity();
    Entity parent = world.createEntity();

    HierarchySystem hierarchySystem(world.components());

    hierarchySystem.setParent(child, parent);

    std::optional<Entity> optionalParent = hierarchySystem.getParent(child);
    REQUIRE(optionalParent.has_value());
    REQUIRE(*optionalParent == parent);
}

TEST_CASE("HierarchySystem returns no parent for a child that has no parent")
{
    World world;
    Entity child = world.createEntity();

    HierarchySystem hierarchySystem(world.components());

    std::optional<Entity> optionalParent = hierarchySystem.getParent(child);
    REQUIRE_FALSE(optionalParent);
}

TEST_CASE("Hierarchy returns all children for a parent with children")
{
    World world;

    Entity childFirst = world.createEntity();
    Entity childSecond = world.createEntity();
    Entity childThird = world.createEntity();

    Entity parent = world.createEntity();

    HierarchySystem hierarchySystem(world.components());

    hierarchySystem.setParent(childFirst, parent);
    hierarchySystem.setParent(childSecond, parent);
    hierarchySystem.setParent(childThird, parent);

    std::vector<Entity> children = hierarchySystem.getChildren(parent);

    REQUIRE(children.size() == 3);

    REQUIRE(std::find(children.begin(), children.end(), childFirst) != children.end());
    REQUIRE(std::find(children.begin(), children.end(), childSecond) != children.end());
    REQUIRE(std::find(children.begin(), children.end(), childThird) != children.end());
}

TEST_CASE("Hierarchy returns no children for a parent with no children")
{
    World world;

    Entity parent = world.createEntity();

    HierarchySystem hierarchySystem(world.components());

    std::vector<Entity> children = hierarchySystem.getChildren(parent);

    REQUIRE(children.empty());
}

TEST_CASE("HierarchySystem can change an entity's parent")
{
    World world;

    Entity child = world.createEntity();
    Entity firstParent = world.createEntity();
    Entity secondParent = world.createEntity();

    HierarchySystem hierarchySystem(world.components());

    hierarchySystem.setParent(child, firstParent);
    hierarchySystem.setParent(child, secondParent);

    std::optional<Entity> parent = hierarchySystem.getParent(child);

    REQUIRE(parent.has_value());
    REQUIRE(*parent == secondParent);

    REQUIRE(hierarchySystem.getChildren(firstParent).empty());

    std::vector<Entity> secondParentChildren = hierarchySystem.getChildren(secondParent);

    REQUIRE(secondParentChildren.size() == 1);
    REQUIRE(secondParentChildren[0] == child);
}

TEST_CASE("HierarchySystem can remove parent from entity without parent")
{
    World world;
    Entity entity = world.createEntity();

    HierarchySystem hierarchySystem(world.components());
    REQUIRE_FALSE(hierarchySystem.hasParent(entity));
    hierarchySystem.removeParent(entity);
    REQUIRE_FALSE(hierarchySystem.hasParent(entity));
}

TEST_CASE("HierarchySystem detects cycle and rejects parenting")
{
    World world;

    Entity A = world.createEntity();
    Entity B = world.createEntity();
    Entity C = world.createEntity();
    
    HierarchySystem hierarchySystem(world.components());

    hierarchySystem.setParent(C, B);
    hierarchySystem.setParent(B, A);

    REQUIRE_THROWS_AS(hierarchySystem.setParent(A, C), std::invalid_argument);
    
    REQUIRE_FALSE(hierarchySystem.hasParent(A));
    REQUIRE(hierarchySystem.getParent(B) == A);
    REQUIRE(hierarchySystem.getParent(C) == B);
}

TEST_CASE("HierarchySystem rejects self parenting")
{
    World world;

    Entity A = world.createEntity();
    
    HierarchySystem hierarchySystem(world.components());

    REQUIRE_THROWS_AS(hierarchySystem.setParent(A, A), std::invalid_argument);
    REQUIRE_FALSE(hierarchySystem.hasParent(A));
}