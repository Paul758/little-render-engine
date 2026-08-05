#pragma once

#include <GLFW/glfw3.h>

#include "Camera.h"
#include "CameraController.h"

class CylinderCameraController : public CameraController
{
private:
    Camera* camera_ = nullptr;
    Vec3 target_{0.0f, 0.0f, 0.0f};
    float yaw_ = 0.0f;
    float radius_ = 5.0f;
    float height_ = 3.0f;

    void updateCameraPosition(Camera& camera);

public:
    void activate(GLFWwindow* window, Camera& camera) override;
    void deactivate(GLFWwindow* window, Camera& camera) override;
    void update(GLFWwindow* window, Camera& camera, float deltaTime) override;


};