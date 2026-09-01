#pragma once
#include "ecs/ComponentRegistry.h"
#include "behaviourtree/PlayerIntent.h"

struct BehaviourContext
{
    Entity entity;
    ComponentRegistry& componentRegistry;
    PlayerIntent& intent;
};