#include <algorithm>


#include "systems/EditorCameraUpdateSystem.h"

#include "components/TransformComponent.h"
#include "components/TransformUtils.h"
#include "components/camera/FreeFlyCameraComponent.h"
#include "components/camera/FreeHandCameraComponent.h"
#include "components/camera/WorldCameraComponent.h"
#include "math/MathUtils.h"
#include "math/Vec2.h"
#include <iostream>

EditorCameraUpdateSystem::EditorCameraUpdateSystem(ComponentRegistry& registry, Entity& entity)
    : registry_(registry), camera(entity)
{

}

void EditorCameraUpdateSystem::updateCamera(Input& input, float deltaTime)
{
    TransformComponent* transform = registry_.get<TransformComponent>(camera);
    FreeFlyCameraComponent* freeFlyComponent = registry_.get<FreeFlyCameraComponent>(camera);
    FreeHandCameraComponent* freeHandComponent = registry_.get<FreeHandCameraComponent>(camera);
    WorldCameraComponent* worldCameraComponent = registry_.get<WorldCameraComponent>(camera);

    if (transform == nullptr || freeFlyComponent == nullptr || freeHandComponent == nullptr || worldCameraComponent == nullptr)
    {
        return;
    }

    if(input.isKeyDown(GLFW_KEY_LEFT_CONTROL))
    {
        input.captureMouse();
    }

    if(input.isKeyReleased(GLFW_KEY_LEFT_CONTROL))
    {
        input.releaseMouse();
    }

    if (input.isKeyDown(GLFW_KEY_LEFT_CONTROL))
    {
        updateFreeFly(input, transform, freeFlyComponent, worldCameraComponent, deltaTime);
    }
    else
    {
        updateFreeHand(input, transform, freeHandComponent, worldCameraComponent, deltaTime);
    }
}

void EditorCameraUpdateSystem::updateFreeFly(Input& input, TransformComponent* transform, FreeFlyCameraComponent* freeFlyComponent, WorldCameraComponent* worldCameraComponent, float deltaTime)
{
        Vec3 movement{};

        Vec2 mouseDelta = input.getMouseDelta();
        worldCameraComponent->yaw -= mouseDelta.x * freeFlyComponent->lookSensitivity;
        worldCameraComponent->pitch -= mouseDelta.y * freeFlyComponent->lookSensitivity;

        transform->rotation = Quaternion::fromEuler(Vec3{
            Math::radians(worldCameraComponent->pitch),
            Math::radians(worldCameraComponent->yaw),
            0.0f});

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

        if (input.isKeyDown(GLFW_KEY_SPACE))
        {
            movement += TransformUtils::getUp(*transform);
        }

        if (input.isKeyDown(GLFW_KEY_LEFT_SHIFT))
        {
            movement += -TransformUtils::getUp(*transform);
        }

        if (movement.lengthSquared() > 0.0f)
        {
            movement = movement.normalized();
        }

        transform->position += movement * deltaTime;
}

void EditorCameraUpdateSystem::updateFreeHand(Input& input, TransformComponent* transform, FreeHandCameraComponent* freeHandComponent, WorldCameraComponent* worldCameraComponent, float deltaTime)
{
    Vec3 movement{};

    if (input.isMouseButtonDown(MouseButton::Middle))
    {
        Vec2 mouseDelta = input.getMouseDelta();

        Vec3 forward = TransformUtils::getForward(*transform);
        Vec3 right = TransformUtils::getRight(*transform);
        Vec3 up = TransformUtils::getUp(*transform);

        Vec3 translationHorizontal = right * -mouseDelta.x;
        Vec3 translationVertical = up * mouseDelta.y;
        
        Vec3 translationVec = translationHorizontal + translationVertical;

        transform->position += translationVec * deltaTime;
    }

    else if (input.isMouseButtonDown(MouseButton::Right))
    {
        Vec2 mouseDelta = input.getMouseDelta();
        worldCameraComponent->yaw -= mouseDelta.x * freeHandComponent->lookSensitivity;
        worldCameraComponent->pitch -= mouseDelta.y * freeHandComponent->lookSensitivity;

        transform->rotation = Quaternion::fromEuler(Vec3{
            Math::radians(worldCameraComponent->pitch),
            Math::radians(worldCameraComponent->yaw),
            0.0f
        });
    }

    else if (input.getScrollDelta() != 0.0f)
    {

        Vec3 forward = TransformUtils::getForward(*transform);
        
        if (input.getScrollDelta() > 0.0f)
        {
            movement += forward * freeHandComponent->scrollSensitivity;
            transform->position += movement;
        }
        else if(input.getScrollDelta() < 0.0f)
        {
            movement += -forward * freeHandComponent->scrollSensitivity;
            transform->position += movement;
        }
    }
}