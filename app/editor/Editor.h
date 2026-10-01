#pragma once

#include "engine/core/ecs/Entity.h"

#include "engine/systems/EditorCameraInputSystem.h"
#include "engine/systems/EditorCameraUpdateSystem.h"

class World;
class HierarchySystem;
class Input;

class Editor
{
public:
    explicit Editor(World& world, HierarchySystem& hierarchySystem);
    ~Editor();

    void initialize();
    void update(Input& input, float deltaTime);

    void drawGui();

    Entity getCamera() const;

private:
    void drawEntityNode(Entity entity);

private:
    World& world_;
    HierarchySystem& hierarchySystem_;
    Entity camera_;
    EditorCameraInputSystem editorCameraInputSystem_;
    EditorCameraUpdateSystem editorCameraUpdateSystem_;
};