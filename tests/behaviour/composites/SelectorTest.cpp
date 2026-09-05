#include <catch2/catch_test_macros.hpp>

#include "behaviour/composites/Selector.h"

#include "ecs/World.h"

#include "fakes/behaviour/FakeNodes.h"

TEST_CASE("Selector returns after one success")
{
    World world;
    Entity entity = world.createEntity();

    auto recordNode = std::make_unique<RecordingNode>();
    auto recordNodePtr = recordNode.get();
    auto successNode = std::make_unique<AlwaysSucceedNode>();

    auto selectorNode = std::make_unique<Selector>();

    selectorNode->addChild(std::move(successNode));
    selectorNode->addChild(std::move(recordNode));

    BehaviourContext context{entity, world.components()};

    NodeState nodeState = selectorNode->update(context);

    REQUIRE(nodeState == NodeState::SUCCESS);
    REQUIRE(recordNodePtr->evaluatedEntities.empty());
}

TEST_CASE("Selector continues after one fail")
{
    World world;
    Entity entity = world.createEntity();

    auto recordNode = std::make_unique<RecordingNode>();
    auto recordNodePtr = recordNode.get();
    auto failNode = std::make_unique<AlwaysFailNode>();

    auto selectorNode = std::make_unique<Selector>();

    selectorNode->addChild(std::move(failNode));
    selectorNode->addChild(std::move(recordNode));

    BehaviourContext context{entity, world.components()};

    NodeState nodeState = selectorNode->update(context);

    REQUIRE(nodeState == NodeState::SUCCESS);
    REQUIRE(recordNodePtr->evaluatedEntities.size() == 1);
    REQUIRE(recordNodePtr->evaluatedEntities[0] == entity);

}

TEST_CASE("Selector returns FAILURE if all children fail")
{
    World world;
    Entity entity = world.createEntity();

    auto selectorNode = std::make_unique<Selector>();

    selectorNode->addChild(std::make_unique<AlwaysFailNode>());
    selectorNode->addChild(std::make_unique<AlwaysFailNode>());

    BehaviourContext context{entity, world.components()};

    NodeState nodeState = selectorNode->update(context);

    REQUIRE(nodeState == NodeState::FAILURE);
}