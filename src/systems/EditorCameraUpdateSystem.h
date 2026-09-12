#pragma once

#include "components/TransformComponent.h"
#include "components/camera/FreeFlyCameraComponent.h"
#include "components/camera/FreeHandCameraComponent.h"
#include "components/camera/WorldCameraComponent.h"
#include "components/camera/EditorCameraInputComponent.h"

#include "ecs/ComponentRegistry.h"
#include "ecs/Entity.h"
#include "Input.h"

class EditorCameraUpdateSystem
{
public:
    EditorCameraUpdateSystem(ComponentRegistry& registry, Entity& camera);
    void updateCamera(EditorCameraInputComponent& input, float deltaTime);

private:
    void updateFreeFly(EditorCameraInputComponent& input, TransformComponent* transform, FreeFlyCameraComponent* freeFlyComponent, WorldCameraComponent* WorldCameraComponent, float deltaTime);   
    void updateFreeHand(EditorCameraInputComponent& input, TransformComponent* transform, FreeHandCameraComponent* freeHandComponent, WorldCameraComponent* WorldCameraComponent, float deltaTime);

private:
    ComponentRegistry& registry_;
    Entity& camera;
};