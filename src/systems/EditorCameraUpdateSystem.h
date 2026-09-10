#pragma once


#include "ecs/ComponentRegistry.h"
#include "ecs/Entity.h"
#include "Input.h"

class EditorCameraUpdateSystem
{
public:
    EditorCameraUpdateSystem(ComponentRegistry& registry, Entity& camera);
    void updateCamera(Input& input, float deltaTime);
private:
    ComponentRegistry& registry_;
    Entity& camera;
};