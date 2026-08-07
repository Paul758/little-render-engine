
#include <iostream>

#include "CylinderCameraController.h"
#include "math/MathUtils.h"

void CylinderCameraController::activate(GLFWwindow* window, Camera& camera)
{
    glfwSetCursorPosCallback(window, nullptr);
    camera_ = &camera;
    camera.setProjectionMode(ProjectionMode::Orthographic);
    updateCameraPosition(camera);
}

void CylinderCameraController::update(GLFWwindow* window, Camera& camera, float deltaTime)
{
    constexpr float rotationSpeed = 60.0f;

    if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        std::cout << "In CylinderCameraController now rotating left \n";
        yaw_ -= rotationSpeed * deltaTime;
    }

    if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        std::cout << "In CylinderCameraController now rotating right \n";
        yaw_ += rotationSpeed * deltaTime;
    }

    updateCameraPosition(camera);
}

void CylinderCameraController::updateCameraPosition(Camera& camera)
{
    const float yawRadians = Math::radians(yaw_);

    const Vec3 position {
        target_.x + std::sin(yawRadians) * radius_,
        target_.y + height_,
        target_.z + std::cos(yawRadians) * radius_
    };

    camera.setPosition(position);
    camera.setTarget(target_);

}

void CylinderCameraController::deactivate(GLFWwindow* window, Camera& camera)
{

}