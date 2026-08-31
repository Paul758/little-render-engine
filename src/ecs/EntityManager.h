#pragma once
#include <cstdint>
#include <vector>

#include "Entity.h"

class EntityManager
{
public:
    Entity create();
    void destroy(Entity entity);
    bool isAlive(Entity entity) const;
private:
    std::vector<std::uint32_t> generations;
    std::vector<std::uint32_t> freeIndices;
};