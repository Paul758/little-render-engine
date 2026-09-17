#pragma once

#include "engine/core/ecs/ComponentRegistry.h"
#include "engine/behaviour/BehaviourTreeRegistry.h"

class BehaviourTreeSystem
{
public:
    BehaviourTreeSystem(ComponentRegistry& components, BehaviourTreeRegistry& trees);
    void update();

private:
    ComponentRegistry& components_;
    BehaviourTreeRegistry& trees_;
};