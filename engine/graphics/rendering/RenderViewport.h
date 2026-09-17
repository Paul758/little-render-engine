#pragma once

#include "engine/core/ecs/Entity.h"
#include "engine/graphics/Framebuffer.h"

struct RenderViewport
{
    Entity camera;
    int width;
    int height;

    Framebuffer* target = nullptr;
};