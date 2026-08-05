#pragma once

#include <GLFW/glfw3.h>

#include "Camera.h"
#include "CameraController.h"

class FreeFlyCameraController : public CameraController
{
public:
    void activate(GLFWwindow* window, Camera& camera) override;
    void update(GLFWwindow* window, Camera& camera, float deltaTime) override;
    void deactivate(GLFWwindow* window, Camera& camera) override;
private:
    Camera* camera_ = nullptr;

    Vec3 savedPosition_{0.0f, 0.0f, 3.0f};
    float savedYaw_ = -90.0f;
    float savedPitch_ = 0.0f;

    Vec3 position_{0.0f, 0.0f, 3.0f};
    float yaw_ = -90.0f;
    float pitch_ = 0.0f;

    Vec3 front_{0.0f, 0.0f, -1.0f};
    Vec3 right_{1.0f, 0.0f, 0.0f};
    Vec3 up_{0.0f, 0.0f, 0.0f};
    Vec3 worldUp_{0.0f, 1.0f, 0.0f};

    bool firstMouse_ = true;
    double lastMouseX_ = 0.0;
    double lastMouseY_ = 0.0;

    float mouseSensitivity_ = 0.05f;
    float movementSpeed_ = 5.0f;

    static void mousePositionCallback(GLFWwindow* window, double mouseX, double mouseY);
    void handleMouseMovement(double mouseX, double mouseY);

    void processMouseMovement(float xOffset, float yOffset);

    void moveForward(float deltaTime);
    void moveBackward(float deltaTime);
    void moveLeft(float deltaTime);
    void moveRight(float deltaTime);
    void moveUp(float deltaTime);
    void moveDown(float deltaTime);

    void updateCameraVectors();
    void updateCameraPosition();

};