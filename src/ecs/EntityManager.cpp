#include "EntityManager.h"

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
    }

    return Entity{index, generations[index]};
}

void EntityManager::destroy(Entity entity)
{
    if (!isAlive(entity))
    {
        return;
    }

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