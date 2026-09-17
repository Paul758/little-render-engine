#pragma once

#include <vector>

#include "engine/components/lighting/DirectionalLightData.h"

struct LightingData
{
    Vec3 ambientColor = {0.1f, 0.1f, 0.1f};
    std::vector<DirectionalLightData> directionalLights;

};