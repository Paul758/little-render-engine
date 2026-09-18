#pragma once

#include "engine/core/ecs/ComponentRegistry.h"
#include "engine/components/camera/EditorCameraInputComponent.h"
#include "engine/core/input/Input.h"

class EditorCameraInputSystem
{
public:
    EditorCameraInputSystem(ComponentRegistry& registry);
    void build(Input& input, Entity camera);

private:
    ComponentRegistry& registry_;
};