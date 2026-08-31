#pragma once
#include "ComponentRegistry.h"

class World
{
public:
    Entity createEntity();
    void destroyEntity(Entity entity);
    bool isAlive(Entity entity) const;
    ComponentRegistry& components();

private:
    ComponentRegistry componentRegistry;
    EntityManager entityManager;
};