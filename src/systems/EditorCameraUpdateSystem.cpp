#include <algorithm>


#include "systems/EditorCameraUpdateSystem.h"

#include "components/TransformComponent.h"
#include "components/TransformUtils.h"
#include "components/camera/FreeFlyCameraComponent.h"
#include "components/camera/FreeHandCameraComponent.h"
#include <MathUtils.h>

EditorCameraUpdateSystem::EditorCameraUpdateSystem(ComponentRegistry& registry, Entity& entity)
    : registry_(registry), camera(entity)
{

}

void EditorCameraUpdateSystem::updateCamera(Input& input, float deltaTime)
{
    TransformComponent* transform = registry_.get<TransformComponent>(camera);
    FreeFlyCameraComponent* freeFlyComponent = registry_.get<FreeFlyCameraComponent>(camera);
    FreeHandCameraComponent* freeHandComponent = registry_.get<FreeHandCameraComponent>(camera);
    if (transform == nullptr || freeFlyComponent == nullptr)
    {
        return;
    }

    if(input.isKeyDown(GLFW_KEY_LEFT_CONTROL))
    {
        input.captureMouse();
    }

    if(input.isKeyDown(GLFW_KEY_LEFT_CONTROL))
    {
        input.releaseMouse();
    }

    if (input.isKeyDown(GLFW_KEY_LEFT_CONTROL))
    {
        updateFreeFly(input, transform, freeFlyComponent, deltaTime);
    }
    else
    {
        updateFreeHand(input, transform, freeHandComponent, deltaTime);
    }
}

void updateFreeFly(Input& input, TransformComponent* transform, FreeFlyCameraComponent* freeFlyComponent, float deltaTime)
{
        Vec3 movement{};

        Vec2 mouseDelta = input.getMouseDelta();
        freeFlyComponent->yaw += mouseDelta.x * freeFlyComponent->lookSensitivity;
        freeFlyComponent->pitch += mouseDelta.y * freeFlyComponent->lookSensitivity;

        freeFlyComponent->pitch = std::clamp(freeFlyComponent->pitch, -89.0f, 89.0f);

        transform->rotation = Vec3{Math::radians(freeFlyComponent->yaw), Math::radians(freeFlyComponent->pitch), 0.0f};

        if (input.isKeyDown(GLFW_KEY_W))
        {
            movement += TransformUtils::getForward(*transform); 
        }

        if (input.isKeyDown(GLFW_KEY_A))
        {
            movement += -TransformUtils::getRight(*transform); 
        }

        if (input.isKeyDown(GLFW_KEY_S))
        {
            movement += -TransformUtils::getForward(*transform); 
        }

        if (input.isKeyDown(GLFW_KEY_D))
        {
            movement += TransformUtils::getRight(*transform); 
        }

        if (movement.lengthSquared() > 0.0f)
        {
            movement = movement.normalized();
        }

        transform->position += movement * deltaTime;
}

void updateFreeHand(Input& input, TransformComponent* transform, FreeHandCameraComponent* freeHandComponent, float deltaTime)
{
    Vec3 movement{};

    if (input.isMouseButtonDown(MouseButton::Middle))
    {
        Vec2 mouseDelta = input.getMouseDelta();

        movement += Vec3{mouseDelta.x, mouseDelta.y, 0.0f};
        transform->position += movement * deltaTime;
    }

    else if (input.isMouseButtonDown(MouseButton::Right))
    {
        Vec2 mouseDelta = input.getMouseDelta();
        freeHandComponent->yaw += mouseDelta.x * freeHandComponent->lookSensitivity;
        freeHandComponent->pitch += mouseDelta.y * freeHandComponent->lookSensitivity;

        freeHandComponent->pitch = std::clamp(freeHandComponent->pitch, -89.0f, 89.0f);

        transform->rotation = Vec3{Math::radians(freeHandComponent->yaw), Math::radians(freeHandComponent->pitch), 0.0f};

    }

    else if (input.getScrollDelta() != 0.0f)
    {
        Vec3 forward = TransformUtils::getForward(*transform);
        
        if (input.getScrollDelta() > 0.0f)
        {
            movement += forward * freeHandComponent->scrollSensitivity;
        }
        else if(input.getScrollDelta() < 0.0f)
        {
            movement += -forward * freeHandComponent->scrollSensitivity;
        }
    }
}