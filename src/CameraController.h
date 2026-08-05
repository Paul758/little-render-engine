#pragma once
#include <GLFW/glfw3.h>

#include "Camera.h"

class CameraController
{
public:
    virtual ~CameraController() = default;

    virtual void activate(GLFWwindow* window, Camera& camera) = 0;
    virtual void deactivate(GLFWwindow* window, Camera& camera) = 0;
    virtual void update(GLFWwindow* window, Camera& camera, float deltaTime) = 0;
};