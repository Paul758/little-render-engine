#pragma once

#include <vector>
#include "ecs/Entity.h"
#include "math/Vec3.h"

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