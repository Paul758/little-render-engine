#pragma once

#include "components/camera/EditorCameraInputComponent.h"
#include "Input.h"

class EditorCameraInputSystem
{
public:
    EditorCameraInputComponent build(Input& input);
};