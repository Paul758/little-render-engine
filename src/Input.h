#pragma once

#include <array>
#include <GLFW/glfw3.h>

#include "IInput.h"

class Input : public IInput
{
public:
    explicit Input(GLFWwindow* window);
    void update();
    bool isKeyDown(int key) const override;
    bool isKeyPressed(int key) const override;
    bool isKeyReleased(int key) const override;

private:
    GLFWwindow* window_;

    std::array<bool, GLFW_KEY_LAST + 1> currentKeys{};
    std::array<bool, GLFW_KEY_LAST + 1> previousKeys{};
};