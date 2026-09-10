#pragma once

#include "ecs/ComponentRegistry.h"
#include "graphics/RenderView.h"

#include "components/camera/CameraComponent.h"
#include "components/TransformComponent.h"
#include "graphics/RenderViewport.h"

class CameraSystem
{
public:
    CameraSystem(ComponentRegistry& registry);
    RenderView buildRenderView(const RenderViewport& viewport);

private:
    Mat4 buildProjectionMatrix(
        const CameraComponent& camera,
        float aspectRatio) const;

    CameraBasis buildCameraBasis(
        const TransformComponent& transform
    ) const;

private:
    ComponentRegistry& registry_;
};