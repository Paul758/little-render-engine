#pragma once

#include "PixelFramebuffer.h"
#include "Mesh.h"
#include "graphics/ShaderProgram.h"

struct GLFWwindow;

class PixelRenderer
{
public:
    PixelRenderer(int width, int height, ShaderProgram& postProcessShader);
    void beginFrame() const;
    void present(GLFWwindow* window) const;

private:
    PixelFramebuffer frameBuffer_;
    Mesh screenMesh_;
    ShaderProgram* postProcessShader_;
};