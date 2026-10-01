#include <stdexcept>
#include <algorithm>
#include <vector>

#include "engine/systems/HierarchySystem.h"
#include "engine/core/ecs/Entity.h"
#include "engine/core/ecs/ComponentRegistry.h"
#include "engine/components/ParentComponent.h"

HierarchySystem::HierarchySystem(ComponentRegistry& registry)
    : registry_(registry)
{

}

void HierarchySystem::setParent(Entity child, Entity parent)
{
    if (wouldCreateCycle(child, parent))
    {
        throw std::invalid_argument("Cannot set parent: hierarchy cycle detected");
    }

    ParentComponent* retrievedParent = registry_.get<ParentComponent>(child);

    if (retrievedParent != nullptr)
    {
        retrievedParent->parent = parent;
    }
    else
    {
        registry_.add<ParentComponent>(child, ParentComponent{parent});
    }

}

void HierarchySystem::removeParent(Entity child)
{
    registry_.remove<ParentComponent>(child);
}

bool HierarchySystem::hasParent(Entity child) const
{
    return registry_.has<ParentComponent>(child);
}

std::optional<Entity> HierarchySystem::getParent(Entity child) const
{
    ParentComponent* retrievedParent = registry_.get<ParentComponent>(child);

    if (retrievedParent == nullptr)
    {
        return std::nullopt;
    }
    return retrievedParent->parent;
}

std::vector<Entity> HierarchySystem::getChildren(Entity parent) const
{
    std::vector<Entity> result;
    std::vector<Entity> entities = registry_.getEntitiesWith<ParentComponent>();

    for (Entity entity : entities)
    {
        const ParentComponent* retrievedParent = registry_.get<ParentComponent>(entity);
        if (retrievedParent->parent == parent)
        {
            result.push_back(entity);
        }
    }

    return result;
}

bool HierarchySystem::wouldCreateCycle(Entity child, Entity parent) const
{
    std::vector<Entity> visited;

    Entity current = parent;

    while (true)
    {
        if (current == child)
        {
            return true;
        }

        if (std::find(visited.begin(), visited.end(), current) != visited.end())
        {
            return true;
        }

        visited.push_back(current);

        std::optional<Entity> currentParent = getParent(current);

        if (!currentParent)
        {
            return false;
        }

        current = *currentParent;
    }
}