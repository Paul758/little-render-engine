#pragma once
#include "engine/core/ecs/ComponentRegistry.h"
#include "engine/graphics/rendering/Renderview.h"

struct LightingData;

class RenderSystem
{
public:
    RenderSystem(ComponentRegistry& registry);
    void render(const RenderView& view, const LightingData& lightingData);
private:
    ComponentRegistry& registry_;
};