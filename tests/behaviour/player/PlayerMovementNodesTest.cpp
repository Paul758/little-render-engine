#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include "ecs/World.h"
#include "ecs/Entity.h"

#include "components/PlayerInputComponent.h"

#include "behaviour/BehaviourContext.h"
#include "behaviour/player/PlayerMovementNodes.h"

using Catch::Approx;

TEST_CASE("CheckHasInputComponent succeeds for an entity with PlayerInputComponent")
{
    World world;
    Entity entity = world.createEntity();

    PlayerInputComponent playerInputComponent{};

    world.components().add<PlayerInputComponent>(entity, playerInputComponent);

    BehaviourContext context{entity, world.components()};

    auto checkHasMovementInputNode = std::make_unique<CheckHasInputComponent>();

    NodeState nodeState = checkHasMovementInputNode->update(context);

    REQUIRE(nodeState == NodeState::SUCCESS);
}

TEST_CASE("CheckHasInputComponent fails for an entity without a PlayerInputComponent")
{
    World world;
    Entity entity = world.createEntity();

    BehaviourContext context{entity, world.components()};

    auto checkHasMovementInputNode = std::make_unique<CheckHasInputComponent>();

    NodeState nodeState = checkHasMovementInputNode->update(context);

    REQUIRE(nodeState == NodeState::FAILURE);
}

TEST_CASE("TaskSetMovementIntentNode modifies the locomotionIntentComponent for a valid entity")
{
    World world;
    Entity entity = world.createEntity();

    PlayerInputComponent playerInputComponent{};
    LocomotionIntentComponent locomotionIntentComponent{};

    playerInputComponent.movement = Vec3{1.25f, -2.0f, 3.75f};

    world.components().add<PlayerInputComponent>(entity, playerInputComponent);
    world.components().add<LocomotionIntentComponent>(entity, locomotionIntentComponent);

    auto taskSetMovementIntentNode = std::make_unique<TaskSetMovementIntent>();

    BehaviourContext context{entity, world.components()};

    NodeState nodeState = taskSetMovementIntentNode->update(context);

    auto* intentComponent = world.components().get<LocomotionIntentComponent>(entity);

    REQUIRE(nodeState == NodeState::SUCCESS);
    REQUIRE(intentComponent != nullptr);
    REQUIRE(intentComponent->movementDirection.x == Approx(1.25f));
    REQUIRE(intentComponent->movementDirection.y == Approx(-2.0f));
    REQUIRE(intentComponent->movementDirection.z == Approx(3.75f));
}

TEST_CASE("TaskSetMovementIntentNode fails for an entity without LocomotionIntentComponent")
{
    World world;
    Entity entity = world.createEntity();

    PlayerInputComponent playerInputComponent{};

    playerInputComponent.movement = Vec3{1.0f, 1.0f, 1.0f};

    world.components().add<PlayerInputComponent>(entity, playerInputComponent);

    auto taskSetMovementIntentNode = std::make_unique<TaskSetMovementIntent>();

    BehaviourContext context{entity, world.components()};

    NodeState nodeState = taskSetMovementIntentNode->update(context);

    REQUIRE(nodeState == NodeState::FAILURE);
}

TEST_CASE("TaskSetMovementIntentNode fails for an entity without PlayerInputComponent")
{
    World world;
    Entity entity = world.createEntity();

    LocomotionIntentComponent locomotionIntentComponent{};

    world.components().add<LocomotionIntentComponent>(entity, locomotionIntentComponent);

    auto taskSetMovementIntentNode = std::make_unique<TaskSetMovementIntent>();

    BehaviourContext context{entity, world.components()};

    NodeState nodeState = taskSetMovementIntentNode->update(context);

    REQUIRE(nodeState == NodeState::FAILURE);
}