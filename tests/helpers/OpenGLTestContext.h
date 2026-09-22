#pragma once

struct GLFWwindow;

class OpenGLTestContext
{
public:
    OpenGLTestContext();
    ~OpenGLTestContext();

    OpenGLTestContext(const OpenGLTestContext&) = delete;
    OpenGLTestContext& operator=(const OpenGLTestContext) = delete;

private:
    GLFWwindow* window_ = nullptr;
};