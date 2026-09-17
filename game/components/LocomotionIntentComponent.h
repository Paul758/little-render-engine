#pragma once

#include "engine/math/Vec3.h"

struct LocomotionIntentComponent
{
    Vec3 movementDirection{};
    bool wantsToJump = false;
};