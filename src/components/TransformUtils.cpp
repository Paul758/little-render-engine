#include "components/TransformUtils.h"

namespace TransformUtils
{
    Vec3 getForward(const TransformComponent& transform)
    {
        return transform.rotation.rotate(Vec3 {0.0f, 0.0f, -1.0f});
    }

    Vec3 getRight(const TransformComponent& transform)
    {
        return transform.rotation.rotate(Vec3 {1.0f, 0.0f, 0.0f});
    }

    Vec3 getUp(const TransformComponent& transform)
    {
        return transform.rotation.rotate(Vec3 {0.0f, 1.0f, 0.0f});
    }
}

