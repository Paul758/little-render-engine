#pragma once
#include "math/Vec3.h"

struct PlayerIntent
{
    Vec3 movementDirection{};
    bool wantsToJump = false;
    bool wantsToThrow = false;
};