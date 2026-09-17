#include "Renderer.h"
#include <glad/gl.h>

#include "engine/graphics/rendering/RenderViewport.h"
#include "engine/graphics/rendering/Renderview.h"
#include "engine/graphics/Framebuffer.h"

#include "engine/systems/CameraSystem.h"
#include "engine/systems/RenderSystem.h"
#include "engine/systems/LightingSystem.h"

#include "engine/components/lighting/LightingData.h"

Renderer::Renderer(RenderSystem& renderSystem, CameraSystem& cameraSystem, LightingSystem& lightSystem)
    : renderSystem_(renderSystem), cameraSystem_(cameraSystem), lightSystem_(lightSystem)
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

    LightingData lighting = lightSystem_.buildLightingData();

    renderSystem_.render(view, lighting);
}