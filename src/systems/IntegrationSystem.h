#pragma once

#include "ecs/ComponentRegistry.h"
#include "systems/CommandBuffer.h"
#include "GameTime.h"

class IntegrationSystem
{
public:
    IntegrationSystem(ComponentRegistry& registry);
    void update(float deltaTime);

private:
    ComponentRegistry& registry_;
};