#pragma once

#include "math/Vec3.h"
#include "GameObject.h"
#include "OrbitCameraController.h"
#include "Input.h"

class PlayerController
{
private:
    Vec3 forward_{1.0f, 0.0f, 0.0f};
    Vec3 worldUp_{0.0f, 1.0f, 0.0f};
    Vec3 right_{Vec3::cross(forward_, worldUp_)};

    OrbitCameraController& cameraController_;

    float movementSpeed_ = 5.0f;

public:
    PlayerController(OrbitCameraController& cameraController);
    void update(Input& input, GameObject& player, float deltaTime);
};