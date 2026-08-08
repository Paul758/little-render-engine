

#include "Input.h"

Input::Input(GLFWwindow* window) : window_(window)
{
}

bool Input::isKeyPressed(int key) const
{
    return (glfwGetKey(window_, key) == GLFW_PRESS);
}