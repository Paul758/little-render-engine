#pragma once

#include "ecs/ComponentRegistry.h"
#include "systems/CommandBuffer.h"

class LocomotionSystem
{
public:
    LocomotionSystem(ComponentRegistry& registry);
    void update(const CommandBuffer& commands);

private:
    void applyMovementCommands(const CommandBuffer& commands);

private:
    ComponentRegistry& registry_;

};