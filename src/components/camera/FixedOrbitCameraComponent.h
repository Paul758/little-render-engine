#pragma once

#include <vector>
#include "components/camera/OrbitPose.h"
#include "math/Vec3.h"

struct FixedOrbitCameraComponent
{
    Vec3 target{0.0f, 0.0f, 0.0f};
    std::vector<OrbitPose> poses;
    std::size_t selectedPose = 0;
    float rotationSpeed = 8.0f;
    float radius = 10.0f;
    
};