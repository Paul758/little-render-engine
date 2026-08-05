#include <iostream>

#include "FreeFlyCameraController.h"
#include "math/MathUtils.h"


void FreeFlyCameraController::activate(GLFWwindow* window, Camera& camera)
{
    camera_ = &camera;
    firstMouse_ = true;

    glfwSetWindowUserPointer(window, this);
    glfwSetCursorPosCallback(window, FreeFlyCameraController::mousePositionCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void FreeFlyCameraController::mousePositionCallback(GLFWwindow* window, double mouseX, double mouseY)
{
    FreeFlyCameraController* controller = static_cast<FreeFlyCameraController*>(glfwGetWindowUserPointer(window));
    if (controller == nullptr)
    {
        std::cout << "There is no camera controller";
        return;
    }

    controller -> handleMouseMovement(mouseX, mouseY);
}

void FreeFlyCameraController::handleMouseMovement(double mouseX, double mouseY)
{
    if (camera_ == nullptr) {
        std::cout << "There is no camera";
        return;
    }

    if (firstMouse_)
    {
        lastMouseX_ = mouseX;
        lastMouseY_ = mouseY;
        firstMouse_ = false;
        return;
    }

    const float xOffset = static_cast<float>(mouseX - lastMouseX_);
    const float yOffset = static_cast<float>(lastMouseY_ - mouseY);

    lastMouseX_ = mouseX;
    lastMouseY_ = mouseY;

    processMouseMovement(xOffset, yOffset);
}

void FreeFlyCameraController::processMouseMovement(float xOffset, float yOffset)
{
    yaw_ += xOffset * mouseSensitivity_;
    pitch_ += yOffset * mouseSensitivity_;

    if (pitch_ > 89.0f)
    {
        pitch_ = 89.0f;
    }

    if (pitch_ < -89.0f)
    {
        pitch_ = -89.0f;
    }
    updateCameraVectors();

}

void FreeFlyCameraController::updateCameraVectors()
{
    const float yawRadians = Math::radians(yaw_);
    const float pitchRadians = Math::radians(pitch_);

    Vec3 direction {
        std::cos(yawRadians) * std::cos(pitchRadians),
        std::sin(pitchRadians),
        std::sin(yawRadians) * std::cos(pitchRadians)
    };

    front_ = direction.normalized();
    std::cout << "Front is " << "x: " << front_.x << ", y: " << front_.y << ", z: " << front_.z << "\n";
    right_ = Vec3::cross(front_, worldUp_).normalized();
    up_ = Vec3::cross(right_, front_).normalized();

}


void FreeFlyCameraController::update(GLFWwindow* window, Camera& camera, float deltaTime)
{
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        moveForward(deltaTime);
    }

    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        moveBackward(deltaTime);
    }

    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        moveLeft(deltaTime);
    }

    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        moveRight(deltaTime);
    }

    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        moveUp(deltaTime);
    }   

    if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        moveDown(deltaTime);
    }

    camera.setPosition(position_);
    camera.setTarget(position_ + front_);
}


void FreeFlyCameraController::moveForward(float deltaTime)
{
    position_ += front_ * movementSpeed_ * deltaTime;
}

void FreeFlyCameraController::moveBackward(float deltaTime)
{
    position_ -= front_ * movementSpeed_ * deltaTime;
}

void FreeFlyCameraController::moveLeft(float deltaTime)
{
    position_ -= right_ * movementSpeed_ * deltaTime;
}

void FreeFlyCameraController::moveRight(float deltaTime)
{
    position_ += right_ * movementSpeed_ * deltaTime;
}

void FreeFlyCameraController::moveUp(float deltaTime)
{
    position_ += worldUp_ * movementSpeed_ * deltaTime;
}

void FreeFlyCameraController::moveDown(float deltaTime)
{
    position_ -= worldUp_ * movementSpeed_ * deltaTime;
}

void FreeFlyCameraController::deactivate(GLFWwindow* window, Camera& camera)
{
    
}