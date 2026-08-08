#include <iostream>

#include "OrbitCameraController.h"
#include "math/MathUtils.h"

void OrbitCameraController::activate(GLFWwindow* window, Camera& camera)
{
    glfwSetCursorPosCallback(window, nullptr);
    camera_ = &camera;
    camera.setProjectionMode(ProjectionMode::Orthographic);
    updateCameraPosition(camera);
}

void OrbitCameraController::update(GLFWwindow* window, Camera& camera, float deltaTime)
{
    const bool qPressed = glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS;
    const bool ePressed = glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS;

    const bool rotating = std::abs(targetYaw_ - yaw_) > 0.01f;

    if (!rotating)
    {
        if(qPressed && !qWasPressed_)
        {
            //std::cout << "In OrbitCameraController now rotating left \n";
            targetYaw_ -= 90.0f;
        }

        if(ePressed && !eWasPressed_)
        {
            //std::cout << "In OrbitCameraController now rotating right \n";
            targetYaw_ += 90.0f;
        }
    }
    

    qWasPressed_ = qPressed;
    eWasPressed_ = ePressed;

    const float blend = 1.0f - std::exp(-rotationSpeed_ * deltaTime);

    yaw_ += (targetYaw_ - yaw_) * blend;

    if (std::abs(targetYaw_ - yaw_) < 0.01f)
    {
        yaw_ = targetYaw_;
    }

    updateCameraPosition(camera);
    updateCameraVectors();
}

void OrbitCameraController::updateCameraPosition(Camera& camera)
{
    const float yawRadians = Math::radians(yaw_);
    const float pitchRadians = Math::radians(pitch_);

    const Vec3 offset {
        radius_ * std::cos(pitchRadians) * std::sin(yawRadians),
        radius_ * std::sin(pitchRadians),
        radius_ * std::cos(pitchRadians) * std::cos(yawRadians)
    };

    position_ = target_ + offset;

    camera.setPosition(target_ + offset);
    camera.setTarget(target_);


}

void OrbitCameraController::updateCameraVectors()
{
    Vec3 temp = target_ - position_;

    Vec3 forwardXZ {temp.x, 0, temp.z};

    forward_ = forwardXZ.normalized();
    right_ = Vec3::cross(forward_, worldUp_).normalized();
}

const Vec3& OrbitCameraController::getForward() const
{
    return forward_;
}

const Vec3& OrbitCameraController::getRight() const
{
    return right_;
}


void OrbitCameraController::deactivate(GLFWwindow* window, Camera& camera)
{

}