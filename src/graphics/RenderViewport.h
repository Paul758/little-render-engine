#pragma once

#include "ecs/Entity.h"
#include "graphics/Framebuffer.h"

struct RenderViewport
{
    Entity camera;
    int width;
    int height;

    Framebuffer* target = nullptr;
};