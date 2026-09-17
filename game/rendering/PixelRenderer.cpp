#include <GLFW/glfw3.h>

#include "game/rendering/PixelRenderer.h"
#include "engine/graphics/PrimitiveMesh.h"


PixelRenderer::PixelRenderer(int width, int height, ShaderProgram& postProcessShader) : 
    frameBuffer_(width, height),
    screenMesh_(PrimitiveMesh::quad()),
    postProcessShader_(&postProcessShader)
{
}

void PixelRenderer::beginFrame() const
{
    frameBuffer_.bind();
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.08f, 0.10f, 0.14f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void PixelRenderer::present(GLFWwindow* window) const
{
    PixelFramebuffer::unbind();

    int windowWidth = 0;
    int windowHeight = 0;

    glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
    glViewport(0, 0, windowWidth, windowHeight);

    glDisable(GL_DEPTH_TEST);

    postProcessShader_->use();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, frameBuffer_.getColorTexture());
    postProcessShader_->setInt("screenTexture", 0);
    screenMesh_.draw();
}