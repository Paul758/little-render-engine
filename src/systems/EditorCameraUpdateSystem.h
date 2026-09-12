#pragma once

#include "components/TransformComponent.h"
#include "components/camera/FreeFlyCameraComponent.h"
#include "components/camera/FreeHandCameraComponent.h"
#include "components/camera/WorldCameraComponent.h"

#include "ecs/ComponentRegistry.h"
#include "ecs/Entity.h"
#include "Input.h"

class EditorCameraUpdateSystem
{
public:
    EditorCameraUpdateSystem(ComponentRegistry& registry, Entity& camera);
    void updateCamera(Input& input, float deltaTime);

private:
    void updateFreeFly(Input& input, TransformComponent* transform, FreeFlyCameraComponent* freeFlyComponent, WorldCameraComponent* WorldCameraComponent, float deltaTime);   
    void updateFreeHand(Input& input, TransformComponent* transform, FreeHandCameraComponent* freeHandComponent, WorldCameraComponent* WorldCameraComponent, float deltaTime);

private:
    ComponentRegistry& registry_;
    Entity& camera;
};