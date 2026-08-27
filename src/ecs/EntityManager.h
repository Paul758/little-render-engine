#pragma once
#include <cstdint>

using Entity = std::uint32_t;

class EntityManager
{
public:
    Entity createEntity();
    void deleteEntity();
private:
    Entity livingEntitiesCount{};
    const Entity maxEntityCount{10000};
};