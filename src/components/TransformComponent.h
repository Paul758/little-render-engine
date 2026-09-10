#pragma once

#include "math/Vec3.h"
#include "math/Mat4.h"
#include "math/Quaternion.h"

struct TransformComponent
{
    Vec3 position{0.0f, 0.0f, 0.0f};
    Quaternion rotation;
    Vec3 scale{1.0f, 1.0f, 1.0f};

    Mat4 getModelMatrix() const;
};