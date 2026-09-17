#pragma once

#include "engine/math/Vec3.h"
#include "engine/math/Mat4.h"
#include "engine/math/Quaternion.h"

struct TransformComponent
{
    Vec3 position{0.0f, 0.0f, 0.0f};
    Quaternion rotation;
    Vec3 scale{1.0f, 1.0f, 1.0f};

    Mat4 getModelMatrix() const;
};