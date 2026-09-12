#pragma once

#include "Input.h"

#include "components/camera/EditorCameraInputComponent.h"

#include "systems/EditorCameraInputSystem.h"
#include "systems/EditorCameraUpdateSystem.h"

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