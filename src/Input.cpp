

#include "Input.h"
#include <iostream>

Input::Input(GLFWwindow* window) : window_(window)
{
    glfwSetWindowUserPointer(window_, this);
    glfwSetScrollCallback(window_, scrollCallback);

    glfwSetCursorPosCallback(window_, cursorPositionCallback);
}

void Input::update()
{
    previousKeys = currentKeys;

    for (int key = 0; key <= GLFW_KEY_LAST; ++key)
    {
        currentKeys[key] = glfwGetKey(window_, key) == GLFW_PRESS;
    }

    previousMouseButtons = currentMouseButtons;

    currentMouseButtons[static_cast<std::size_t>(MouseButton::Left)] =
        glfwGetMouseButton(window_, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;

    currentMouseButtons[static_cast<std::size_t>(MouseButton::Right)] =
        glfwGetMouseButton(window_, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;
    
    currentMouseButtons[static_cast<std::size_t>(MouseButton::Middle)] =
        glfwGetMouseButton(window_, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS;
}

bool Input::isKeyDown(int key) const
{
    return currentKeys[key];
}

bool Input::isKeyPressed(int key) const
{
    return currentKeys[key] && !previousKeys[key];
}

bool Input::isKeyReleased(int key) const
{
    return !currentKeys[key] && previousKeys[key];
}

std::size_t Input::toIndex(MouseButton button) const
{
    return static_cast<std::size_t>(button);
}

bool Input::isMouseButtonDown(MouseButton button) const
{
    return currentMouseButtons[toIndex(button)];
}

bool Input::isMouseButtonPressed(MouseButton button) const
{
    return currentMouseButtons[toIndex(button)] && !previousMouseButtons[toIndex(button)];
}

bool Input::isMouseButtonReleased(MouseButton button) const
{
    return !currentMouseButtons[toIndex(button)] && previousMouseButtons[toIndex(button)];
}

float Input::getScrollDelta() const
{
    return scrollDelta;
}

void Input::addScrollDelta(float delta)
{
    scrollDelta += delta;
}

void Input::beginFrame()
{
    scrollDelta = 0.0f;
    mouseDelta = Vec2{0.0f, 0.0f};
}

void Input::cursorPositionCallback(GLFWwindow* window, double x, double y)
{
    auto* input = static_cast<Input*>(glfwGetWindowUserPointer(window));

    if (input == nullptr)
    {
        return;
    }

    Vec2 newPosition{static_cast<float>(x), static_cast<float>(y)};

    if (!input->hasMousePosition)
    {
        input->mousePosition = newPosition;
        input->hasMousePosition = true;
        return;
    }

    input->mouseDelta += newPosition - input->mousePosition;
    input->mousePosition = newPosition;
}

Vec2 Input::getMouseDelta() const
{
    return mouseDelta;
}

void Input::captureMouse()
{
    glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Input::releaseMouse()
{
    glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    resetMousePosition();
}

void Input::resetMousePosition()
{
    hasMousePosition = false;
    mouseDelta = Vec2{};
}