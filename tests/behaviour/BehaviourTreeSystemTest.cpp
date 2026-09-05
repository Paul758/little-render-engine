#include <catch2/catch_test_macros.hpp>

#include <vector>
#include <algorithm>

#include "behaviour/BehaviourTreeRegistry.h"

#include "ecs/World.h"

#include "components/BehaviourTreeComponent.h"

#include "systems/BehaviourTreeSystem.h"

#include "fakes/behaviour/FakeNodes.h"

TEST_CASE("BehaviourSystem evaluates entities with a BehaviourComponent")
{
    World world;
    BehaviourTreeRegistry treeRegistry;

    auto node = std::make_unique<RecordingNode>();
    RecordingNode* nodePtr = node.get();

    auto tree = std::make_unique<BehaviourTree>(std::move(node));

    BehaviourTreeId treeId = treeRegistry.add(std::move(tree));

    Entity entity = world.createEntity();

    world.components().add<BehaviourTreeComponent>(entity, BehaviourTreeComponent{treeId});

    BehaviourTreeSystem system(world.components(), treeRegistry);

    system.update();

    REQUIRE(nodePtr->evaluatedEntities.size() == 1);
    REQUIRE(nodePtr->evaluatedEntities[0] == entity);

}

TEST_CASE("BehaviourSystem does not evaluate an entity without a BehaviourTreeComponent")
{
    World world;
    BehaviourTreeRegistry treeRegistry;

    auto node = std::make_unique<RecordingNode>();
    RecordingNode* nodePtr = node.get();

    auto tree = std::make_unique<BehaviourTree>(std::move(node));

    BehaviourTreeId treeId = treeRegistry.add(std::move(tree));

    Entity entity = world.createEntity();

    BehaviourTreeSystem system(world.components(), treeRegistry);

    system.update();

    REQUIRE(nodePtr->evaluatedEntities.empty());

}

TEST_CASE("BehaviourSystem evaluates multiple entities with the same tree")
{
    World world;
    BehaviourTreeRegistry treeRegistry;

    auto node = std::make_unique<RecordingNode>();
    RecordingNode* nodePtr = node.get();

    BehaviourTreeId treeId = treeRegistry.add(std::make_unique<BehaviourTree>(std::move(node)));

    Entity first = world.createEntity();
    Entity second = world.createEntity();

    world.components().add<BehaviourTreeComponent>(first, BehaviourTreeComponent{treeId});
    world.components().add<BehaviourTreeComponent>(second, BehaviourTreeComponent{treeId});
    
    BehaviourTreeSystem system(world.components(), treeRegistry);

    system.update();

    REQUIRE(nodePtr->evaluatedEntities.size() == 2);

    REQUIRE(std::find(
        nodePtr->evaluatedEntities.begin(),
        nodePtr->evaluatedEntities.end(),
        first) != nodePtr->evaluatedEntities.end());

    REQUIRE(std::find(
        nodePtr->evaluatedEntities.begin(),
        nodePtr->evaluatedEntities.end(),
        second) != nodePtr->evaluatedEntities.end());
}

TEST_CASE("BehaviourSystem evaluates two different trees")
{
    World world;
    BehaviourTreeRegistry treeRegistry;

    auto firstNode = std::make_unique<RecordingNode>();
    auto secondNode = std::make_unique<RecordingNode>();

    RecordingNode* firstNodePtr = firstNode.get();
    RecordingNode* secondNodePtr = secondNode.get();

    BehaviourTreeId firstTreeId = treeRegistry.add(std::make_unique<BehaviourTree>(std::move(firstNode)));
    BehaviourTreeId secondTreeId = treeRegistry.add(std::make_unique<BehaviourTree>(std::move(secondNode)));

    Entity firstEntity = world.createEntity();
    Entity secondEntity = world.createEntity();

    world.components().add<BehaviourTreeComponent>(firstEntity, BehaviourTreeComponent{firstTreeId});
    world.components().add<BehaviourTreeComponent>(secondEntity, BehaviourTreeComponent{secondTreeId});

    BehaviourTreeSystem system(world.components(), treeRegistry);

    system.update();

    REQUIRE(firstNodePtr->evaluatedEntities.size() == 1);
    REQUIRE(firstNodePtr->evaluatedEntities[0] == firstEntity);

    REQUIRE(secondNodePtr->evaluatedEntities.size() == 1);
    REQUIRE(secondNodePtr->evaluatedEntities[0] == secondEntity);
}

TEST_CASE("BehaviourTreeSystem provides the evaluated entity's components through the context")
{
    World world;
    BehaviourTreeRegistry treeRegistry;

    BehaviourTreeId treeId = treeRegistry.add(std::make_unique<BehaviourTree>(std::make_unique<ModifyTestComponentNode>()));
    
    Entity entity = world.createEntity();

    world.components().add<TestComponent>(entity, TestComponent{});

    world.components().add<BehaviourTreeComponent>(entity, BehaviourTreeComponent{treeId});

    BehaviourTreeSystem system(world.components(), treeRegistry);

    system.update();

    TestComponent* component = world.components().get<TestComponent>(entity);

    REQUIRE(component != nullptr);
    REQUIRE(component->value == 42);
}

TEST_CASE("BehaviourTreeSystem ignores an invalid BehaviourTreeId")
{
    World world;
    BehaviourTreeRegistry treeRegistry;

    Entity entity = world.createEntity();

    world.components().add<BehaviourTreeComponent>(entity, BehaviourTreeComponent{999});

    BehaviourTreeSystem system(world.components(), treeRegistry);

    REQUIRE_NOTHROW(system.update());
}