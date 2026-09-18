#pragma once

#include "engine/core/ecs/Entity.h"

#include "engine/systems/EditorCameraInputSystem.h"
#include "engine/systems/EditorCameraUpdateSystem.h"

class World;
class Input;

class Editor
{
public:
    explicit Editor(World& world);
    ~Editor();

    void initialize();
    void update(Input& input, float deltaTime);
    Entity getCamera() const;

private:
    World& world_;
    Entity camera_;
    EditorCameraInputSystem editorCameraInputSystem_;
    EditorCameraUpdateSystem editorCameraUpdateSystem_;
};