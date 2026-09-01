

#include "Input.h"

Input::Input(GLFWwindow* window) : window_(window)
{
}

void Input::update()
{
    previousKeys = currentKeys;

    for (int key = 0; key <= GLFW_KEY_LAST; ++key)
    {
        currentKeys[key] = glfwGetKey(window_, key) == GLFW_PRESS;
    }
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