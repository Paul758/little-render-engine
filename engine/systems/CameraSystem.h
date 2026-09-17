#pragma once

#include "engine/core/ecs/ComponentRegistry.h"
#include "engine/graphics/rendering/RenderView.h"

#include "engine/components/camera/CameraComponent.h"
#include "engine/components/TransformComponent.h"
#include "engine/graphics/rendering/RenderViewport.h"

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