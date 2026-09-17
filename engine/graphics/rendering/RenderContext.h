#pragma once

#include "engine/graphics/rendering/Renderview.h"
#include "engine/components/TransformComponent.h"
#include "engine/components/lighting/LightingData.h"

struct RenderContext
{
    const RenderView& view;
    const TransformComponent& transform;
    const LightingData& lighting;
};