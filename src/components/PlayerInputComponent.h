#pragma once
#include "math/Vec3.h"

struct PlayerInputComponent
{
    Vec3 movement{};

    bool jumpPressed = false;
    bool throwPressed = false;
};