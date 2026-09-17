#include "engine/graphics/rendering/ScreenRenderer.h"
#include "engine/graphics/ShaderProgram.h"

ScreenRenderer::ScreenRenderer(ShaderProgram& shader)
    : shader_(shader)
{
    const float vertices[] = {
        //position      //uv
        -1.0f, -1.0f,   0.0f, 0.0f,
         1.0f, -1.0f,   1.0f, 0.0f,
         1.0f, 1.0f,    1.0f, 1.0f,
         
        -1.0f, -1.0f,   0.0f, 0.0f,
         1.0f, 1.0f,    1.0f, 1.0f,
        -1.0f, 1.0f,    0.0f, 1.0f
    };

    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);

    glBindVertexArray(vao_);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //position
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void*>(0));

    glEnableVertexAttribArray(0);

    //texture coordinates
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 *sizeof(float), reinterpret_cast<void*>(2 * sizeof(float)));

    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

ScreenRenderer::~ScreenRenderer()
{
    glDeleteBuffers(1, &vbo_);
    glDeleteVertexArrays(1, &vao_);

}

void ScreenRenderer::drawTexture(GLuint texture, int x, int y, int width, int height)
{
    glViewport(x, y, width, height);

    shader_.use();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    shader_.setInt("screenTexture", 0);

    glBindVertexArray(vao_);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
}