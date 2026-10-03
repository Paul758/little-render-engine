#pragma once

#include "engine/core/ecs/Entity.h"
#include "engine/math/Vec3.h"
#include <optional>

class TransformComponent;

class TransformEditor
{
public:
    void draw(TransformComponent& transform);

private:
};