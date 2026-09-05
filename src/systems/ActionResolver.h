#pragma once

#include "ecs/ComponentRegistry.h"
#include "systems/CommandBuffer.h"

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