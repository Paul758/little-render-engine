#pragma once

#include "engine/core/input/Input.h"

#include "engine/components/camera/EditorCameraInputComponent.h"

#include "engine/systems/EditorCameraInputSystem.h"
#include "engine/systems/EditorCameraUpdateSystem.h"

class EditorSystem
{
public:
    EditorSystem(ComponentRegistry& registry, Entity& camera);
    void update(Input& input, float deltaTime);

private:
    Entity& camera_;
    ComponentRegistry& registry_;
    EditorCameraInputComponent editorCameraInputComponent{};
    EditorCameraInputSystem editorCameraInputSystem{};
    EditorCameraUpdateSystem editorCameraUpdateSystem{registry_, camera_};
};