#include <catch2/catch_test_macros.hpp>

#include <vector>

#include "behaviour/BehaviourTreeRegistry.h"
#include "behaviour/player/PlayerMovementBTreeBuilder.h"
#include "behaviour/composites/Sequence.h"

#include "ecs/World.h"

#include "components/BehaviourTreeComponent.h"

#include "systems/BehaviourTreeSystem.h"

#include "fakes/behaviour/FakeNodes.h"

TEST_CASE("Sequence returns SUCCESS when nodes succeed")
{
    World world;

    Entity entity = world.createEntity();

    BehaviourContext context{entity, world.components()};

    auto sequence = std::make_unique<Sequence>();

    auto succeedNodeFirst = std::make_unique<AlwaysSucceedNode>();
    auto succeedNodeSecond = std::make_unique<AlwaysSucceedNode>();

    sequence->addChild(std::move(succeedNodeFirst));
    sequence->addChild(std::move(succeedNodeSecond));

    NodeState result = sequence->update(context);

    REQUIRE(result == NodeState::SUCCESS);
}

TEST_CASE("Sequence returns FAILURE when nodes fail")
{
    World world;

    Entity entity = world.createEntity();

    BehaviourContext context{entity, world.components()};

    auto sequence = std::make_unique<Sequence>();
    auto failNode = std::make_unique<AlwaysFailNode>();
    sequence->addChild(std::move(failNode));

    NodeState result = sequence->update(context);

    REQUIRE(result == NodeState::FAILURE);
}

TEST_CASE("Tree Sequence stops if Node returns FAILURE")
{
    World world;
    BehaviourTreeRegistry treeRegistry;

    auto failSequence = std::make_unique<Sequence>();

    auto succeedNode = std::make_unique<AlwaysSucceedNode>();
    auto failNode = std::make_unique<AlwaysFailNode>();

    auto recordingNode = std::make_unique<RecordingNode>();
    auto* recordNodePtr = recordingNode.get();

    failSequence->addChild(std::move(succeedNode));
    failSequence->addChild(std::move(failNode));
    failSequence->addChild(std::move(recordingNode));

    auto tree = std::make_unique<BehaviourTree>(std::move(failSequence));

    BehaviourTreeId treeId = treeRegistry.add(std::move(tree));

    Entity entity = world.createEntity();

    world.components().add<BehaviourTreeComponent>(entity, BehaviourTreeComponent{treeId});

    BehaviourTreeSystem system(world.components(), treeRegistry);

    system.update();

    REQUIRE(recordNodePtr->evaluatedEntities.empty());
}

TEST_CASE("Tree Sequence runs through if Node returns SUCCESS")
{
    World world;
    BehaviourTreeRegistry treeRegistry;

    auto succeedSequence = std::make_unique<Sequence>();

    auto succeedNodeFirst = std::make_unique<AlwaysSucceedNode>();
    auto succeedNodeSecond = std::make_unique<AlwaysSucceedNode>();

    auto recordingNode = std::make_unique<RecordingNode>();
    auto* recordNodePtr = recordingNode.get();

    succeedSequence->addChild(std::move(succeedNodeFirst));
    succeedSequence->addChild(std::move(succeedNodeSecond));
    succeedSequence->addChild(std::move(recordingNode));

    auto tree = std::make_unique<BehaviourTree>(std::move(succeedSequence));

    BehaviourTreeId treeId = treeRegistry.add(std::move(tree));

    Entity entity = world.createEntity();

    world.components().add<BehaviourTreeComponent>(entity, BehaviourTreeComponent{treeId});

    BehaviourTreeSystem system(world.components(), treeRegistry);

    system.update();

    REQUIRE(recordNodePtr->evaluatedEntities.size() == 1);
    REQUIRE(recordNodePtr->evaluatedEntities[0] == entity);
}