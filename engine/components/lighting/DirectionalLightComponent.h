#pragma once

#include "engine/math/Vec3.h"

struct DirectionalLightComponent
{
    Vec3 color{1.0f, 1.0f, 1.0f};
    float intensity = 1.0f;
};