#pragma once

#include "engine/core/ecs/ComponentRegistry.h"
#include "game/systems/CommandBuffer.h"

class ActionResolver
{
public:
    explicit ActionResolver(ComponentRegistry& components);
    void update(CommandBuffer& commands);

private:
    void resolveLocomotion(CommandBuffer& commands);
    void resolveCombat(CommandBuffer& commands);

    ComponentRegistry& components_;
};