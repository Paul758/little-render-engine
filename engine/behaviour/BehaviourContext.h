#pragma once
#include "engine/core/ecs/ComponentRegistry.h"

struct BehaviourContext
{
    Entity entity;
    ComponentRegistry& componentRegistry;
    
    template<typename T>
    T* get()
    {
        return componentRegistry.get<T>(entity);
    }
};