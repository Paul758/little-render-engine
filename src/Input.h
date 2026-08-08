#pragma once

#include <GLFW/glfw3.h>

class Input
{
public:
    explicit Input(GLFWwindow* window);
    bool isKeyPressed(int key) const;

private:
    GLFWwindow* window_;

};