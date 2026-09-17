#pragma once

#include "engine/components/camera/EditorCameraInputComponent.h"
#include "engine/core/input/Input.h"

class EditorCameraInputSystem
{
public:
    EditorCameraInputComponent build(Input& input);
};