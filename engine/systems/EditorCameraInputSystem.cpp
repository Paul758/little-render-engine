

#include "engine/systems/EditorCameraInputSystem.h"

EditorCameraInputSystem::EditorCameraInputSystem(ComponentRegistry& registry)
    : registry_(registry)
{
}

void EditorCameraInputSystem::build(Input& input, Entity camera)
{
    EditorCameraInputComponent* result = registry_.get<EditorCameraInputComponent>(camera);

    if (result == nullptr)
    {
        std::cout << "EditorCameraInputComponent is missing";
        return;
    }

    *result = {};

    result->freeFly = input.isKeyDown(GLFW_KEY_LEFT_CONTROL);

    if (input.isKeyDown(GLFW_KEY_LEFT_CONTROL))
    {
        input.captureMouse();
    }

    if (input.isKeyReleased(GLFW_KEY_LEFT_CONTROL))
    {
        input.releaseMouse();
    }

    if (result->freeFly)
    {
        if (input.isKeyDown(GLFW_KEY_W))
        {
            result->movement.z += 1.0f;
        }

        if (input.isKeyDown(GLFW_KEY_S))
        {
            result->movement.z -= 1.0f;
        }

        if (input.isKeyDown(GLFW_KEY_A))
        {
            result->movement.x -= 1.0f;
        }

        if (input.isKeyDown(GLFW_KEY_D))
        {
            result->movement.x += 1.0f;
        }

        if (input.isKeyDown(GLFW_KEY_SPACE))
        {
            result->movement.y += 1.0f;
        }

        if (input.isKeyDown(GLFW_KEY_LEFT_SHIFT))
        {
            result->movement.y -= 1.0f;
        }

        result->lookDelta = input.getMouseDelta();
    }
    else
    {
        if (input.isMouseButtonDown(MouseButton::Right))
        {
            result->panDelta = input.getMouseDelta();
        }

        if (input.isMouseButtonDown(MouseButton::Middle))
        {
            result->lookDelta = input.getMouseDelta();
        }

        result->zoomDelta = input.getScrollDelta();
    }  
}