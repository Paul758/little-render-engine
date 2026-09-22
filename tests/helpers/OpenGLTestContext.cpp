#include "OpenGLTestContext.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <iostream>

OpenGLTestContext::OpenGLTestContext()
{
    if (!glfwInit())
    {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

    window_ = glfwCreateWindow(1, 1, "OpenGL Test Context", nullptr, nullptr);

    if (window_ == nullptr)
    {
        glfwTerminate();

        throw std::runtime_error("Failed to create GLFW test window");
    }

    glfwMakeContextCurrent(window_);

    const int version = gladLoadGL(glfwGetProcAddress);

    if (version == 0)
    {
        std::cerr << "Failed to initialize GLAD\n";
        glfwDestroyWindow(window_);
        window_ = nullptr;
        glfwTerminate();

        return;
    }
}

OpenGLTestContext::~OpenGLTestContext()
{
    if (window_ != nullptr)
    {
        glfwDestroyWindow(window_);
    }

    glfwTerminate();
}