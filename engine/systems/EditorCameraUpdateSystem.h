#pragma once

#include "engine/components/TransformComponent.h"
#include "engine/components/camera/FreeFlyCameraComponent.h"
#include "engine/components/camera/FreeHandCameraComponent.h"
#include "engine/components/camera/WorldCameraComponent.h"
#include "engine/components/camera/EditorCameraInputComponent.h"

#include "engine/core/ecs/ComponentRegistry.h"
#include "engine/core/ecs/Entity.h"

class EditorCameraUpdateSystem
{
public:
    EditorCameraUpdateSystem(ComponentRegistry& registry);
    void updateCamera(float deltaTime, Entity camera);

private:
    void updateFreeFly(EditorCameraInputComponent* input, TransformComponent* transform, FreeFlyCameraComponent* freeFlyComponent, WorldCameraComponent* WorldCameraComponent, float deltaTime);   
    void updateFreeHand(EditorCameraInputComponent* input, TransformComponent* transform, FreeHandCameraComponent* freeHandComponent, WorldCameraComponent* WorldCameraComponent, float deltaTime);

private:
    ComponentRegistry& registry_;
};