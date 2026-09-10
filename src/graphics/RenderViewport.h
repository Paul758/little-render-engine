#pragma once

#include "ecs/Entity.h"

struct RenderViewport
{
    Entity camera;
    int width;
    int height;
};