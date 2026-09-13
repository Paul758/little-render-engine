#pragma once
#include <glad/gl.h>

class ShaderProgram;

class ScreenRenderer
{
public:
    ScreenRenderer(ShaderProgram& shader);
    ~ScreenRenderer();
    ScreenRenderer(const ScreenRenderer&) = delete;
    ScreenRenderer& operator=(const ScreenRenderer&) = delete;

    void drawTexture(GLuint texture, int x, int y, int width, int height);

private:
    ShaderProgram& shader_;
    GLuint vao_ = 0;
    GLuint vbo_ = 0;
    
};