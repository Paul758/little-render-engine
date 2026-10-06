#pragma once

#include <cstdint>
#include <vector>

#include "engine/serialization/SerializedValue.h"

using SceneEntityId = std::uint64_t;

struct SceneEntityData
{
    SceneEntityId id = 0;
    SerializedValue components;
};

struct SceneData
{
    std::vector<SceneEntityData> entities;
};
