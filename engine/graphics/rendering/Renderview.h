#pragma once

#include "engine/math/Mat4.h"
#include "engine/math/Vec3.h"

struct CameraBasis
{
    Vec3 forward;
    Vec3 right;
    Vec3 up;
};

struct RenderView
{
    Mat4 view;
    Mat4 projection;

    CameraBasis cameraBasis;
};