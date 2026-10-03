#pragma once

#include "engine/core/ecs/Entity.h"

class World;
class HierarchySystem;
struct EditorContext;

class HierarchyPanel
{
public:
    HierarchyPanel(World& world, HierarchySystem& hierarchySystem);

    void draw(EditorContext& context);

private:
    void drawEntityNode(Entity entity, EditorContext& context);

private:
    World& world_;
    HierarchySystem& hierarchySystem_;
};