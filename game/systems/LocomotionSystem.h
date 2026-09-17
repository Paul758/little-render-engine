#pragma once

#include "engine/core/ecs/ComponentRegistry.h"
#include "game/systems/CommandBuffer.h"

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