#pragma once

#include "math/Vec3.h"
#include "components/TransformComponent.h"

namespace TransformUtils
{
    Vec3 getForward(const TransformComponent& transform);
    Vec3 getRight(const TransformComponent& transform);
    Vec3 getUp(const TransformComponent& transform);
}

