#pragma once

#include "ecs/ComponentRegistry.h"
#include "GameTime.h"

class MovementSystem
{
public:
    MovementSystem(ComponentRegistry& registry);
    void update(float deltaTime);

private:
    ComponentRegistry& registry_;
};