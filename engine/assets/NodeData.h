#pragma once

#include "engine/math/Vec3.h"
#include "engine/math/Quaternion.h"
#include <vector>

struct NodeData
{
    int meshIndex = -1;

    Vec3 translation{0.0f, 0.0f, 0.0f};
    Quaternion rotation{};
    Vec3 scale{1.0f, 1.0f, 1.0f};

    std::vector<size_t> children;

    Mat4 makeLocalTransform(const NodeData& node)
    {
        return Mat4::translate(translation) * Mat4::fromQuaternion(rotation) * Mat4::scale(scale);
    }
};