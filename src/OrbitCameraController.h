#pragma once

#include <GLFW/glfw3.h>

#include "Camera.h"
#include "CameraController.h"

class OrbitCameraController : public CameraController
{
private:
    Camera* camera_ = nullptr;
    Vec3 target_{0.0f, 0.0f, 0.0f};

    float yaw_ = 45.0f;
    float targetYaw_ = 45.0f;

    float pitch_ = 30.0f;
    float radius_ = 20.0f;

    float rotationSpeed_ = 8.0f;

    bool qWasPressed_ = false;
    bool eWasPressed_ = false;

    void updateCameraPosition(Camera& camera);

public:
    void activate(GLFWwindow* window, Camera& camera) override;
    void deactivate(GLFWwindow* window, Camera& camera) override;
    void update(GLFWwindow* window, Camera& camera, float deltaTime) override;


};