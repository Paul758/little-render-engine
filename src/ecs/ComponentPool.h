#pragma once

#include <cstddef>
#include <cstdint>
#include <unordered_map>
#include <utility>
#include <vector>

#include "EntityManager.h"
#include "IComponentPool.h"

template<typename T>
class ComponentPool : public IComponentPool
{
public:
    T& add(Entity entity, const T& component)
    {
        const std::size_t index = components.size();
        entityToIndex[entity.index] = index;

        entities.push_back(entity);
        components.push_back(component);

        return components.back();
    }

    T* get(Entity entity)
    {
        auto it = entityToIndex.find(entity.index);

        if (it == entityToIndex.end())
        {
            return nullptr;
        }

        const std::size_t componentIndex = it->second;

        if (componentIndex >= entities.size())
        {
            return nullptr;
        }

        if (entities[componentIndex] != entity)
        {
            return nullptr;
        }

        return &components[componentIndex];
    }

    bool has(Entity entity) const
    {
        auto it = entityToIndex.find(entity.index);

        if (it == entityToIndex.end())
        {
            return false;
        }

        const std::size_t componentIndex = it->second;
        
        return entities[componentIndex] == entity;
    }

    void remove(Entity entity) override
    {
        auto it = entityToIndex.find(entity.index);

        if (it == entityToIndex.end()) 
        {
            return;
        }

        const std::size_t removedIndex = it->second;

        if (entities[removedIndex] != entity)
        {
            return;
        }

        const std::size_t lastIndex = components.size() - 1;

        if (removedIndex != lastIndex)
        {
            components[removedIndex] = std::move(components[lastIndex]);

            entities[removedIndex] = entities[lastIndex];

            const Entity movedEntity = entities[removedIndex];
            entityToIndex[movedEntity.index] = removedIndex;
        }

        components.pop_back();
        entities.pop_back();
        entityToIndex.erase(entity.index);
    }

    const std::vector<Entity>& getEntities() const
    {
        return entities;
    }

private:
    std::vector<Entity> entities;
    std::vector<T> components;
    std::unordered_map<std::uint32_t, std::size_t> entityToIndex;
};