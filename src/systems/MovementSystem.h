#pragma once

#include "ecs/ComponentRegistry.h"
#include "GameTime.h"

class MovementSystem
{
public:
    MovementSystem(ComponentRegistry registry, GameTime gameTime);
    void update();

private:
    ComponentRegistry registry_;
    GameTime time_;
};