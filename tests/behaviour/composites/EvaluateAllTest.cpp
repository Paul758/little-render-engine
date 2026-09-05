#include <catch2/catch_test_macros.hpp>

#include "ecs/World.h"
#include "ecs/Entity.h"

#include "behaviour/composites/EvaluateAll.h"
#include "fakes/behaviour/FakeNodes.h"

TEST_CASE("EvaluateAll evaluates all children")
{
    World world;
    Entity entity = world.createEntity();

    auto evaluateAllNode = std::make_unique<EvaluateAll>();

    auto recordingNodeFirst = std::make_unique<RecordingNode>();
    auto recordingNodeSecond = std::make_unique<RecordingNode>();
    auto recordingNodeThird = std::make_unique<RecordingNode>();

    auto recordingNodeFirstPtr = recordingNodeFirst.get();
    auto recordingNodeSecondPtr = recordingNodeSecond.get();
    auto recordingNodeThirdPtr = recordingNodeThird.get();

    evaluateAllNode->addChild(std::move(recordingNodeFirst));
    evaluateAllNode->addChild(std::move(recordingNodeSecond));
    evaluateAllNode->addChild(std::move(recordingNodeThird));

    BehaviourContext behaviourContext{entity, world.components()};

    NodeState nodeState = evaluateAllNode->update(behaviourContext);

    REQUIRE(nodeState == NodeState::SUCCESS);

    REQUIRE(recordingNodeFirstPtr->evaluatedEntities.size() == 1);
    REQUIRE(recordingNodeFirstPtr->evaluatedEntities[0] == entity);

    REQUIRE(recordingNodeSecondPtr->evaluatedEntities.size() == 1);
    REQUIRE(recordingNodeSecondPtr->evaluatedEntities[0] == entity);

    REQUIRE(recordingNodeThirdPtr->evaluatedEntities.size() == 1);
    REQUIRE(recordingNodeThirdPtr->evaluatedEntities[0] == entity);
}

TEST_CASE("EvaluateAll continues after a child returns FAILURE")
{
    World world;
    Entity entity = world.createEntity();

    auto evaluateAllNode = std::make_unique<EvaluateAll>();

    auto failNode = std::make_unique<AlwaysFailNode>();

    auto recordingNodeFirst = std::make_unique<RecordingNode>();
    auto recordingNodeSecond = std::make_unique<RecordingNode>();
    auto recordingNodeThird = std::make_unique<RecordingNode>();

    auto recordingNodeFirstPtr = recordingNodeFirst.get();
    auto recordingNodeSecondPtr = recordingNodeSecond.get();
    auto recordingNodeThirdPtr = recordingNodeThird.get();

    evaluateAllNode->addChild(std::move(failNode));
    evaluateAllNode->addChild(std::move(recordingNodeFirst));
    evaluateAllNode->addChild(std::move(recordingNodeSecond));
    evaluateAllNode->addChild(std::move(recordingNodeThird));

    BehaviourContext behaviourContext{entity, world.components()};

    NodeState nodeState = evaluateAllNode->update(behaviourContext);

    REQUIRE(nodeState == NodeState::SUCCESS);

    REQUIRE(recordingNodeFirstPtr->evaluatedEntities.size() == 1);
    REQUIRE(recordingNodeFirstPtr->evaluatedEntities[0] == entity);

    REQUIRE(recordingNodeSecondPtr->evaluatedEntities.size() == 1);
    REQUIRE(recordingNodeSecondPtr->evaluatedEntities[0] == entity);

    REQUIRE(recordingNodeThirdPtr->evaluatedEntities.size() == 1);
    REQUIRE(recordingNodeThirdPtr->evaluatedEntities[0] == entity);
}

TEST_CASE("Empty EvaluateAll return SUCCESS")
{
    World world;
    Entity entity = world.createEntity();

    auto evaluateAllNode = std::make_unique<EvaluateAll>();

    BehaviourContext behaviourContext{entity, world.components()};

    NodeState nodeState = evaluateAllNode->update(behaviourContext);

    REQUIRE(nodeState == NodeState::SUCCESS);
}