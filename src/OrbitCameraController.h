#pragma once

#include <GLFW/glfw3.h>

#include "Camera.h"
#include "CameraController.h"

class OrbitCameraController : public CameraController
{
private:
    Camera* camera_ = nullptr;
    Vec3 target_{0.0f, 0.0f, 0.0f};
    Vec3 position_{0.0f, 0.0f, 0.0f};

    float yaw_ = 45.0f;
    float targetYaw_ = 45.0f;

    float pitch_ = 30.0f;
    float radius_ = 20.0f;

    float rotationSpeed_ = 8.0f;

    bool qWasPressed_ = false;
    bool eWasPressed_ = false;

    Vec3 forward_{target_ - position_};
    Vec3 worldUp_{0.0f, 1.0f, 0.0f};
    Vec3 right_{Vec3::cross(forward_, worldUp_)};

    void updateCameraPosition(Camera& camera);
    void updateCameraVectors();

public:
    void activate(GLFWwindow* window, Camera& camera) override;
    void deactivate(GLFWwindow* window, Camera& camera) override;
    void update(GLFWwindow* window, Camera& camera, float deltaTime) override;

    const Vec3& getForward() const;
    const Vec3& getRight() const;
    
};