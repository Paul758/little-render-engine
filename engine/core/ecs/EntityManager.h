#pragma once
#include <cstdint>
#include <vector>
#include <unordered_map>
#include <map>
#include "engine/core/ecs/Entity.h"

class EntityManager
{
public:
    Entity create();
    void destroy(Entity entity);
    bool isAlive(Entity entity) const;
    const std::vector<Entity>& getAliveEntities() const;
private:
    std::vector<std::uint32_t> generations;
    std::vector<std::uint32_t> freeIndices;
    std::vector<Entity> aliveEntities;
    std::vector<std::uint32_t> entityToDenseIndex;
};