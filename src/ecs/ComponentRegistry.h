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

    void removeAll(Entity entity)
    {
        for (auto& [type, pool] : componentPools)
        {
            pool->remove(entity);
        }
    }

    template<typename First, typename... Rest>
    std::vector<Entity> getEntitiesWith()
    {
        std::vector<Entity> result;

        ComponentPool<First>* firstPool = getPool<First>();

        if (firstPool == nullptr)
        {
            return result;
        }

        for (Entity entity : firstPool->getEntities())
        {
            if ((has<Rest>(entity) && ...))
            {
                result.push_back(entity);
            }
        }

        return result;
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

    template<typename T>
    ComponentPool<T>* getPool()
    {
        const std::type_index type = typeid(T);
        auto it = componentPools.find(type);

        if (it == componentPools.end())
        {
            return nullptr;
        }

        return static_cast<ComponentPool<T>*>(it->second.get());
    }


private:
   std::unordered_map<std::type_index, std::unique_ptr<IComponentPool>> componentPools; 
};