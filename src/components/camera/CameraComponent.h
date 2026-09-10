#pragma once

#include "math/Vec3.h"

enum class ProjectionType
{
    Perspective,
    Orthographic
};

struct CameraComponent
{
    ProjectionType projectionType;
    float orthographicSize = 0.0f;
    float fieldOfView = 45.0f;
    float nearPlane = 0.1f;
    float farPlane = 100.0f;
};