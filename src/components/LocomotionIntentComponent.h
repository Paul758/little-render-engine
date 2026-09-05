#pragma once

#include "math/Vec3.h"

struct LocomotionIntentComponent
{
    Vec3 movementDirection{};
    bool wantsToJump = false;
};