#pragma once
#include "engine/core/ecs/ComponentRegistry.h"

class World
{
public:
    Entity createEntity();
    void destroyEntity(Entity entity);
    bool isAlive(Entity entity) const;
    ComponentRegistry& components();
    const ComponentRegistry& components() const;
    const std::vector<Entity>& getAliveEntities() const;

private:
    ComponentRegistry componentRegistry;
    EntityManager entityManager;
};