

#include "engine/core/ecs/World.h"


Entity World::createEntity()
{
    return entityManager.create();
}

void World::destroyEntity(Entity entity)
{
    componentRegistry.removeAll(entity);
    entityManager.destroy(entity);
}

bool World::isAlive(Entity entity) const
{
    return entityManager.isAlive(entity);
}

ComponentRegistry& World::components()
{
    return componentRegistry;
}

const std::vector<Entity>& World::getAliveEntities() const
{
    return entityManager.getAliveEntities();
}

