#pragma once

#include "engine/math/Vec2.h"
#include "engine/math/Vec3.h"

struct EditorCameraInputComponent
{
    Vec2 lookDelta{};
    Vec2 panDelta{};
    Vec3 movement{};
    float zoomDelta = 0.0f;

    bool freeFly = false;
};