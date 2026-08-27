#pragma once
#include "EntityManager.h"

template<typename T>
class ComponentPool : public IComponentPool
{
public:
    T& add(Entity entity, const T& component)
    {
        const std::size_t index = components.size();
        entityToIndex[entity] = index;

        entities.push_back(entity);
        components.push_back(component);

        return components.back();
    }

    T* get(Entity entity)
    {
        auto it = entityToIndex.find(entity);

        if (it == entityToIndex.end())
        {
            return nullptr;
        }

        return &components[it->second];
    }

    bool has(Entity entity) const
    {
        return entityToIndex.contains(entity);
    }

    void remove(Entity entity)
    {
        auto it = entityToIndex.find(entity);

        if (it == entityToIndex.end()) 
        {
            return;
        }

        const std::size_t removedIndex = it->second;
        const std::size_t lastIndex = components.size() - 1;

        if (removedIndex != lastIndex)
        {
            components[removedIndex] = std::move(components[lastIndex]);

            entities[removedIndex] = entities[lastIndex];

            const Entity movedEntity = entities[removedIndex];
            entityToIndex[movedEntity] = removedIndex;
        }

        components.pop_back();
        entities.pop_back();
        entityToIndex.erase(entity);
    }

private:
    std::vector<Entity> entities;
    std::vector<T> components;
    std::unordered_map<Entity, std::size_t> entityToIndex;
};