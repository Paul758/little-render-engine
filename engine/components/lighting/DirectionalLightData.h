#pragma once

#include "engine/math/Vec3.h"

struct DirectionalLightData
{
    Vec3 direction;
    Vec3 color;
    float intensity;
};