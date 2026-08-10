#pragma once

#include "math/Vec3.h"
#include "math/Vec2.h"

struct Vertex
{
    Vec3 position;
    Vec3 color;
    Vec2 texCoord;
    Vec2 localPosition;
};