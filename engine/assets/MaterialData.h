#pragma once

#include "engine/math/Vec4.h"

struct MaterialData
{
    Vec4 baseColor = {1.0f, 1.0f, 1.0f, 1.0f};

    int baseColorTextureIndex = -1;
};