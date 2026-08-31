#pragma once
#include "ecs/ComponentRegistry.h"

#include "graphics/Renderview.h"

class RenderSystem
{
public:
    RenderSystem(ComponentRegistry& registry);
    void render(const RenderView& view);

private:
    ComponentRegistry& registry_;
};