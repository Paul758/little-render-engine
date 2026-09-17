#pragma once

#include "engine/core/time/GameTime.h"
#include "engine/core/ecs/ComponentRegistry.h"

#include "game/systems/CommandBuffer.h"

class IntegrationSystem
{
public:
    IntegrationSystem(ComponentRegistry& registry);
    void update(float deltaTime);

private:
    ComponentRegistry& registry_;
};