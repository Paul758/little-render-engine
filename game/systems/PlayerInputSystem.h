#pragma once

#include "engine/core/ecs/ComponentRegistry.h"
#include "engine/core/input/Input.h"

class PlayerInputSystem
{
public:
    PlayerInputSystem(ComponentRegistry& registry);
    void update(IInput& input);
private:
    ComponentRegistry& registry_;
};