#pragma once

#include <vector>

#include "components/lighting/DirectionalLightData.h"

struct LightingData
{
    std::vector<DirectionalLightData> directionalLights;

};