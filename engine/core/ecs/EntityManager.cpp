#include "engine/core/ecs/EntityManager.h"
#include <vector>

Entity EntityManager::create()
{
    std::uint32_t index;

    if (!freeIndices.empty())
    {
        index = freeIndices.back();
        freeIndices.pop_back();
    }
    else
    {
        index = static_cast<std::uint32_t>(generations.size());
        generations.push_back(0);
        entityToDenseIndex.push_back(0);
    }

    Entity newEntity{index, generations[index]};
    entityToDenseIndex[index] = aliveEntities.size();

    aliveEntities.push_back(newEntity);

    return newEntity;
}

void EntityManager::destroy(Entity entity)
{
    if (!isAlive(entity))
    {
        return;
    }

    const std::size_t removedIndex = entityToDenseIndex[entity.index];
    const std::size_t lastIndex = aliveEntities.size() - 1;

    if (removedIndex != lastIndex)
    {
        const Entity lastEntity = aliveEntities[lastIndex];
        aliveEntities.at(removedIndex) = lastEntity;
        entityToDenseIndex[lastEntity.index] = removedIndex;
    }

    aliveEntities.pop_back();

    ++generations[entity.index];
    freeIndices.push_back(entity.index);
}

bool EntityManager::isAlive(Entity entity) const
{
    if (entity.index >= generations.size())
    {
        return false;
    }

    return generations[entity.index] == entity.generation;
}

const std::vector<Entity>& EntityManager::getAliveEntities() const
{
    return aliveEntities;
}