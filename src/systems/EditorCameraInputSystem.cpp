

#include "systems/EditorCameraInputSystem.h"

EditorCameraInputComponent EditorCameraInputSystem::build(Input& input)
{
    std::cout << "Building the EditorCameraInputComponent";
    EditorCameraInputComponent result{};

    result.freeFly = input.isKeyDown(GLFW_KEY_LEFT_CONTROL);

    if (input.isKeyDown(GLFW_KEY_LEFT_CONTROL))
    {
        input.captureMouse();
    }

    if (input.isKeyReleased(GLFW_KEY_LEFT_CONTROL))
    {
        std::cout << "Releasing mouse";
        input.releaseMouse();
    }

    if (result.freeFly)
    {
        std::cout << "Result is freefly";
        if (input.isKeyDown(GLFW_KEY_W))
        {
            result.movement.z += 1.0f;
        }

        if (input.isKeyDown(GLFW_KEY_S))
        {
            result.movement.z -= 1.0f;
        }

        if (input.isKeyDown(GLFW_KEY_A))
        {
            result.movement.x -= 1.0f;
        }

        if (input.isKeyDown(GLFW_KEY_D))
        {
            result.movement.x += 1.0f;
        }

        if (input.isKeyDown(GLFW_KEY_SPACE))
        {
            result.movement.y += 1.0f;
        }

        if (input.isKeyDown(GLFW_KEY_LEFT_SHIFT))
        {
            result.movement.y -= 1.0f;
        }

        result.lookDelta = input.getMouseDelta();
    }
    else
    {
        std::cout << "Result is not freefly";
        if (input.isMouseButtonDown(MouseButton::Right))
        {
            std::cout << "Mouse button right pressed";
            result.panDelta = input.getMouseDelta();
        }

        if (input.isMouseButtonDown(MouseButton::Middle))
        {
            result.lookDelta = input.getMouseDelta();
        }

        result.zoomDelta = input.getScrollDelta();
    }
    return result;
}