#include <algorithm>
#include <iostream>

#include "engine/systems/EditorCameraUpdateSystem.h"

#include "engine/components/TransformComponent.h"
#include "engine/components/TransformUtils.h"
#include "engine/components/camera/FreeFlyCameraComponent.h"
#include "engine/components/camera/FreeHandCameraComponent.h"
#include "engine/components/camera/WorldCameraComponent.h"
#include "engine/components/camera/EditorCameraInputComponent.h"
#include "engine/math/MathUtils.h"
#include "engine/math/Vec2.h"


EditorCameraUpdateSystem::EditorCameraUpdateSystem(ComponentRegistry& registry, Entity entity)
    : registry_(registry), camera(entity)
{

}

void EditorCameraUpdateSystem::updateCamera(EditorCameraInputComponent& input, float deltaTime)
{
    TransformComponent* transform = registry_.get<TransformComponent>(camera);
    FreeFlyCameraComponent* freeFlyComponent = registry_.get<FreeFlyCameraComponent>(camera);
    FreeHandCameraComponent* freeHandComponent = registry_.get<FreeHandCameraComponent>(camera);
    WorldCameraComponent* worldCameraComponent = registry_.get<WorldCameraComponent>(camera);

    if (transform == nullptr || freeFlyComponent == nullptr || freeHandComponent == nullptr || worldCameraComponent == nullptr)
    {
        std::cout << "components missing" << "\n";
        return;
    }

    if (input.freeFly)
    {
        updateFreeFly(input, transform, freeFlyComponent, worldCameraComponent, deltaTime);
    }
    else
    {
        updateFreeHand(input, transform, freeHandComponent, worldCameraComponent, deltaTime);
    }
}

void EditorCameraUpdateSystem::updateFreeFly(EditorCameraInputComponent& input, TransformComponent* transform, FreeFlyCameraComponent* freeFlyComponent, WorldCameraComponent* worldCameraComponent, float deltaTime)
{
    Vec3 forward = TransformUtils::getForward(*transform);
    Vec3 right = TransformUtils::getRight(*transform);
    Vec3 up = TransformUtils::getUp(*transform);

    Vec3 movement = forward * input.movement.z + right * input.movement.x + up * input.movement.y;

    Vec2 lookDelta = input.lookDelta;
    worldCameraComponent->yaw -= lookDelta.x * freeFlyComponent->lookSensitivity;
    worldCameraComponent->pitch -= lookDelta.y * freeFlyComponent->lookSensitivity;

    float movementSpeed = freeFlyComponent->movementSpeed;

    transform->rotation = Quaternion::fromEuler(Vec3{
        worldCameraComponent->pitch,
        worldCameraComponent->yaw,
        0.0f});

    if (movement.lengthSquared() > 0.0f)
    {
        movement = movement.normalized();
    }

    transform->position += movement * deltaTime;
}

void EditorCameraUpdateSystem::updateFreeHand(EditorCameraInputComponent& input, TransformComponent* transform, FreeHandCameraComponent* freeHandComponent, WorldCameraComponent* worldCameraComponent, float deltaTime)
{
    Vec3 movement{};
    float scroll = input.zoomDelta;

    //Move scene around
    Vec2 mouseDeltaLook = input.lookDelta;
    Vec3 forward = TransformUtils::getForward(*transform);
    Vec3 right = TransformUtils::getRight(*transform);
    Vec3 up = TransformUtils::getUp(*transform);

    Vec3 translationHorizontal = right * -mouseDeltaLook.x;
    Vec3 translationVertical = up * mouseDeltaLook.y;
    Vec3 translationVec = translationHorizontal + translationVertical;

    transform->position += translationVec * deltaTime;
    
    //Pan in scene
    Vec2 mouseDeltaPan = input.panDelta;
    worldCameraComponent->yaw -= mouseDeltaPan.x * freeHandComponent->lookSensitivity;
    worldCameraComponent->pitch -= mouseDeltaPan.y * freeHandComponent->lookSensitivity;

    transform->rotation = Quaternion::fromEuler(Vec3{
        worldCameraComponent->pitch,
        worldCameraComponent->yaw,
        0.0f
    });

    //Zoom in scene
    transform->position += forward * scroll * freeHandComponent->scrollSensitivity;
}