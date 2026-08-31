#pragma once

#include "graphics/Renderview.h"
#include "components/TransformComponent.h"

struct RenderContext
{
    const RenderView& view;
    const TransformComponent& transform;
};