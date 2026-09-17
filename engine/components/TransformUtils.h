#pragma once

#include "engine/math/Vec3.h"
#include "engine/components/TransformComponent.h"

namespace TransformUtils
{
    Vec3 getForward(const TransformComponent& transform);
    Vec3 getRight(const TransformComponent& transform);
    Vec3 getUp(const TransformComponent& transform);
}

