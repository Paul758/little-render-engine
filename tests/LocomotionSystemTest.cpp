#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include "engine/core/ecs/World.h"
#include "engine/core/ecs/Entity.h"

#include "game/components/LocomotionIntentComponent.h"
#include "game/components/VelocityComponent.h"

#include "game/systems/CommandBuffer.h"
#include "game/systems/LocomotionSystem.h"

TEST_CASE("LocomotionSystem applies move commands resulting in modified velocity")
{
    World world;
    Entity entity = world.createEntity();

    world.components().add<VelocityComponent>(entity, VelocityComponent{});

    LocomotionSystem locomotionSystem(world.components());

    CommandBuffer commandBuffer;

    MoveCommand moveCommand{entity, Vec3{1.0f, 2.0f, 3.0f}, 1.0f};
    commandBuffer.moveCommands.push_back(moveCommand);

    locomotionSystem.update(commandBuffer);

    VelocityComponent* velocityRetrieved = world.components().get<VelocityComponent>(entity);

    REQUIRE(velocityRetrieved != nullptr);

    REQUIRE(velocityRetrieved->velocity.x == 1.0f);
    REQUIRE(velocityRetrieved->velocity.z == 3.0f);
}