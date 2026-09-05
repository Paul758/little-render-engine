

#include "components/LocomotionIntentComponent.h"
#include "systems/ActionResolver.h"
#include "ecs/Entity.h"

ActionResolver::ActionResolver(ComponentRegistry& components)
    : components_(components)
{

}

void ActionResolver::update(CommandBuffer& commands)
{
    resolveLocomotion(commands);
    resolveCombat(commands);
}

void ActionResolver::resolveLocomotion(CommandBuffer& commands)
{
    auto entities = components_.getEntitiesWith<LocomotionIntentComponent>();

    for (Entity entity : entities)
    {
        LocomotionIntentComponent* locomotionIntent = components_.get<LocomotionIntentComponent>(entity);

        if (locomotionIntent == nullptr)
        {
            continue;
        }

        
        MoveCommand command;
        command.entity = entity;
        command.direction = locomotionIntent->movementDirection;
        command.desiredSpeed = 5.0f;

        commands.moveCommands.push_back(command);
        
    }
}

void ActionResolver::resolveCombat(CommandBuffer& commands)
{

}