#include "Renderer.h"
#include <glad/gl.h>

#include "graphics/RenderViewport.h"
#include "graphics/Renderview.h"
#include "graphics/Framebuffer.h"

#include "systems/CameraSystem.h"
#include "systems/RenderSystem.h"

Renderer::Renderer(RenderSystem& renderSystem, CameraSystem& cameraSystem)
    : renderSystem_(renderSystem), cameraSystem_(cameraSystem)
{

}

void Renderer::renderViewport(const RenderViewport& viewport)
{
    if (viewport.target != nullptr)
    {
        viewport.target->bind();
    }
    else
    {
        Framebuffer::unbind();
    }

    glViewport(0, 0, viewport.width, viewport.height);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    RenderView view = cameraSystem_.buildRenderView(viewport);
    renderSystem_.render(view);
}