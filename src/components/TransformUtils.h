#pragma once

#include "math/Vec3.h"
#include "components/TransformComponent.h"

namespace TransformUtils
{
    Mat4 getRotationMatrix(const TransformComponent& transform)
    {
        return Mat4::rotate(transform.rotation);
    }

    Vec3 getForward(const TransformComponent& transform)
    {
        Mat4 rotationMatrix = getRotationMatrix(transform);
        Vec3 localForward {0.0f, 0.0f, -1.0f};

        return (rotationMatrix * localForward).normalized();
    }

    Vec3 getRight(const TransformComponent& transform)
    {
        Mat4 rotationMatrix = getRotationMatrix(transform);
        Vec3 localRight {1.0f, 0.0f, 0.0f};

        return (rotationMatrix * localRight).normalized();
    }

    Vec3 getUp(const TransformComponent& transform)
    {
        Mat4 rotationMatrix = getRotationMatrix(transform);
        Vec3 localUp {0.0f, 1.0f, 0.0f};

        return (rotationMatrix * localUp).normalized();
    }
}

