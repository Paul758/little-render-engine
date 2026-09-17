#pragma once

#include <vector>

#include "engine/core/ecs/Entity.h"
#include "engine/math/Vec3.h"

struct MoveCommand
{
    Entity entity;
    Vec3 direction{};
    float desiredSpeed = 0.0f;
};

struct JumpCommand
{
    Entity entity;
    float initialVelocity = 0.0f;
};


class CommandBuffer
{
public:
    void clear();

    std::vector<MoveCommand> moveCommands;
    std::vector<JumpCommand> jumpCommands;
};