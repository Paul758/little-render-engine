#pragma once

#include "graphics/Renderview.h"
#include "components/TransformComponent.h"
#include "components/lighting/LightingData.h"

struct RenderContext
{
    const RenderView& view;
    const TransformComponent& transform;
    const LightingData& lighting;
};