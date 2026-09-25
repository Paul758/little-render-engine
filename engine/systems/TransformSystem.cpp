
#include "engine/systems/TransformSystem.h"
#include "engine/core/ecs/ComponentRegistry.h"
#include "engine/core/ecs/Entity.h"
#include "engine/math/Mat4.h"
#include "engine/systems/HierarchySystem.h"
#include "engine/components/TransformComponent.h"


TransformSystem::TransformSystem(ComponentRegistry& registry, const HierarchySystem& hierarchySystem)
    : registry_(registry), hierarchySystem_(hierarchySystem)
{
}

void TransformSystem::update()
{
    //Fetch roots, i.e. all entities without parent
    std::vector<Entity> entities = registry_.getEntitiesWith<TransformComponent>();

    for (Entity entity : entities)
    {
        if (hierarchySystem_.hasParent(entity))
        {
            continue;
        }

        updateRecursive(entity, Mat4::identity());
    }

}

void TransformSystem::updateRecursive(Entity entity, const Mat4& parentWorld)
{
    TransformComponent* transform = registry_.get<TransformComponent>(entity);

    if (transform == nullptr)
    {
        return;
    }

    const Mat4 local = transform->getLocalMatrix();

    transform->setWorldMatrix(parentWorld * local);

    std::vector<Entity> children = hierarchySystem_.getChildren(entity);

    for(Entity child : children)
    {
        updateRecursive(child, transform->getWorldMatrix());
    }
}