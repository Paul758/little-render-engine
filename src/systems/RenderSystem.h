#pragma once
#include "ecs/ComponentRegistry.h"

#include "graphics/Renderview.h"

struct LightingData;

class RenderSystem
{
public:
    RenderSystem(ComponentRegistry& registry);
    void render(const RenderView& view, const LightingData& lightingData);
private:
    ComponentRegistry& registry_;
};