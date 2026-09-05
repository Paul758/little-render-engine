#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include "ecs/World.h"
#include "ecs/Entity.h"

#include "components/LocomotionIntentComponent.h"

#include "systems/CommandBuffer.h"
#include "systems/ActionResolver.h"

TEST_CASE("ActionResolver does not crate a move command without LocomotionIntentComponent")
{
    World world;
    Entity entity = world.createEntity();

    CommandBuffer commandBuffer;

    ActionResolver actionResolver(world.components());

    actionResolver.update(commandBuffer);

    REQUIRE(commandBuffer.moveCommands.empty());
}


TEST_CASE("ActionResolver resolves a move command")
{
    World world;
    Entity entity = world.createEntity();

    CommandBuffer commandBuffer;

    ActionResolver actionResolver(world.components());

    LocomotionIntentComponent locomotionIntentComponent;

    locomotionIntentComponent.movementDirection = Vec3{1.0f, 2.0f, 3.0f};

    world.components().add<LocomotionIntentComponent>(entity, locomotionIntentComponent);

    actionResolver.update(commandBuffer);

    REQUIRE(commandBuffer.moveCommands.size() == 1);
    REQUIRE(commandBuffer.moveCommands[0].direction == Vec3{1.0f, 2.0f, 3.0f});
}

TEST_CASE("ActionResolver creates move commands for multiple entities")
{
    World world;

    Entity firstEntity = world.createEntity();
    Entity secondEntity = world.createEntity();

    world.components().add<LocomotionIntentComponent>(
        firstEntity,
        LocomotionIntentComponent{Vec3{1.0f, 0.0f, 0.0f}, false});

    world.components().add<LocomotionIntentComponent>(
        secondEntity,
        LocomotionIntentComponent{Vec3{0.0f, 0.0f, 1.0f}, false});
        
    CommandBuffer commandBuffer;
    ActionResolver resolver(world.components());

    resolver.update(commandBuffer);

    REQUIRE(commandBuffer.moveCommands.size() == 2);
    
    auto firstCommand = std::find_if(
        commandBuffer.moveCommands.begin(),
        commandBuffer.moveCommands.end(),
        [firstEntity](const MoveCommand& command)
        {
            return command.entity == firstEntity;
        }
    );
    
    REQUIRE(firstCommand != commandBuffer.moveCommands.end());
    REQUIRE(firstCommand->direction == Vec3{1.0f, 0.0f, 0.0f});

    auto secondCommand = std::find_if(
        commandBuffer.moveCommands.begin(),
        commandBuffer.moveCommands.end(),
        [secondEntity](const MoveCommand& command)
        {
            return command.entity == secondEntity;
        }
    );
    
    REQUIRE(secondCommand != commandBuffer.moveCommands.end());
    REQUIRE(secondCommand->direction == Vec3{0.0f, 0.0f, 1.0f});
}

TEST_CASE("ActionResolver creates a zero direction move command")
{
    World world;
    Entity entity = world.createEntity();

    world.components().add<LocomotionIntentComponent>(
        entity, 
        LocomotionIntentComponent{Vec3{0.0f, 0.0f, 0.0f}, false});
    
    CommandBuffer commandBuffer;
    ActionResolver actionResolver(world.components());

    actionResolver.update(commandBuffer);
    
    REQUIRE(commandBuffer.moveCommands.size() == 1);
    REQUIRE(commandBuffer.moveCommands[0].direction == Vec3{0.0f, 0.0f, 0.0f});
}