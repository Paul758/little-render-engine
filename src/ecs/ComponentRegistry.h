#pragma once

#include <memory>
#include <typeindex>
#include <unordered_map>

#include "ComponentPool.h"
#include "IComponentPool.h"

class ComponentRegistry
{
public:

    template<typename T>
    T& add(Entity entity, const T& component)
    {
        return getOrCreatePool<T>().add(entity, component);
    }

    template<typename T>
    T* get(Entity entity)
    {
        const std::type_index type = typeid(T);

        auto it = componentPools.find(type);

        if (it == componentPools.end())
        {
            return nullptr;
        }

        auto* pool = static_cast<ComponentPool<T>*>(it -> second.get());

        return pool->get(entity);
    }

    template<typename T>
    bool has(Entity entity)
    {
        return get<T>(entity) != nullptr;
    }

    template<typename First, typename... Rest, typename Func>
    void each(Func function)
    {
        ComponentPool<First>* firstPool = getPool<First>();

        if (firstPool == nullptr)
        {
            return;
        }

        for (Entity entity : firstPool->getEntities())
        {
            if ((has<Rest>(entity) && ...))
            {
                function(entity, *get<First>(entity), *get<Rest>(entity)...);
            }
        }
    }

private:
    template<typename T>
    ComponentPool<T>& getOrCreatePool()
    {
        const std::type_index type = typeid(T);

        auto it = componentPools.find(type);

        if (it == componentPools.end())
        {
            auto pool = std::make_unique<ComponentPool<T>>();
            ComponentPool<T>* poolPtr = pool.get();
            componentPools[type] = std::move(pool);

            return *poolPtr;
        }

        return *static_cast<ComponentPool<T>*>(it->second.get());
    }
private:
   std::unordered_map<std::type_index, std::unique_ptr<IComponentPool>> componentPools; 
};