#pragma once

#include "math/Mat4.h"
#include "math/Vec3.h"

struct RenderView
{
    Mat4 view;
    Mat4 projection;

    Vec3 cameraRight;
    Vec3 cameraUp;
};