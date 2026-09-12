#pragma once

#include <array>
#include <iostream>
#include <GLFW/glfw3.h>

#include "IInput.h"
#include "math/Vec2.h"


class Input : public IInput
{
public:
    explicit Input(GLFWwindow* window);
    
    void beginFrame();
    float getScrollDelta() const;
    void addScrollDelta(float delta);

    Vec2 getMousePosition() const;
    Vec2 getMouseDelta() const;

    void update();
    bool isKeyDown(int key) const override;
    bool isKeyPressed(int key) const override;
    bool isKeyReleased(int key) const override;

    bool isMouseButtonDown(MouseButton mouseButton) const override;
    bool isMouseButtonPressed(MouseButton mouseButton) const override;
    bool isMouseButtonReleased(MouseButton mouseButton) const override;

    std::size_t toIndex(MouseButton button) const;

    void captureMouse();
    void releaseMouse();

private:
    static void scrollCallback(GLFWwindow* window, double xOffset, double yOffset)
    {
        auto* input = static_cast<Input*>(glfwGetWindowUserPointer(window));

        if (input == nullptr)
        {
            return;
        }

        input->scrollDelta += static_cast<float>(yOffset);
    }

    static void cursorPositionCallback(GLFWwindow* window, double x, double y);
    void resetMousePosition();

private:
    GLFWwindow* window_;

    std::array<bool, GLFW_KEY_LAST + 1> currentKeys{};
    std::array<bool, GLFW_KEY_LAST + 1> previousKeys{};

    static constexpr std::size_t MouseButtonCount = static_cast<std::size_t>(MouseButton::Count);

    std::array<bool, MouseButtonCount> currentMouseButtons{};
    std::array<bool, MouseButtonCount> previousMouseButtons{};

    float scrollDelta = 0.0f;

    Vec2 mousePosition{};
    Vec2 mouseDelta{};
    bool hasMousePosition = false;

};