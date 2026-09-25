#pragma once

#include "engine/core/ecs/Entity.h"

class ComponentRegistry;
class HierarchySystem;
class Mat4;

class TransformSystem
{
public:
    TransformSystem(ComponentRegistry& registry, const HierarchySystem& hierarchy);
    void update();

private:
    void updateRecursive(Entity entity, const Mat4& parentWorld);

private:
    ComponentRegistry& registry_;
    const HierarchySystem& hierarchySystem_;
};