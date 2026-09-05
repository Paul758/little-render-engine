#pragma once
#include "ecs/ComponentRegistry.h"

class IntentResetSystem
{
public:
    IntentResetSystem(ComponentRegistry& registry);
    void update();
private:
    ComponentRegistry& componentRegistry;

};