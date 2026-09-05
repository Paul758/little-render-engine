
#include "components/VelocityComponent.h"
#include "systems/LocomotionSystem.h"

LocomotionSystem::LocomotionSystem(ComponentRegistry& registry)
    : registry_(registry)
{

}

void LocomotionSystem::update(const CommandBuffer& commands)
{
    applyMovementCommands(commands);
}

void LocomotionSystem::applyMovementCommands(const CommandBuffer& commands)
{
    for (const MoveCommand& command : commands.moveCommands)
    {
        VelocityComponent* velocityComponent = registry_.get<VelocityComponent>(command.entity);

        if (velocityComponent == nullptr)
        {
            continue;
        }

        Vec3 desiredVelocity = command.direction * command.desiredSpeed;
        velocityComponent->velocity.x = desiredVelocity.x;
        velocityComponent->velocity.z = desiredVelocity.z;
    }
}