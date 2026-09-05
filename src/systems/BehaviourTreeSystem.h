#pragma once

#include "ecs/ComponentRegistry.h"
#include "behaviour/BehaviourTreeRegistry.h"

class BehaviourTreeSystem
{
public:
    BehaviourTreeSystem(ComponentRegistry& components, BehaviourTreeRegistry& trees);
    void update();

private:
    ComponentRegistry& components_;
    BehaviourTreeRegistry& trees_;
};