#pragma once

#include "ecs/ComponentRegistry.h"
#include "Input.h"

class InputSystem
{
public:
    InputSystem(ComponentRegistry& registry);
    void update(IInput& input);
private:
    ComponentRegistry& registry_;
};