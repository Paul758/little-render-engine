#include <iostream>

#include "Renderer.h"
#include "graphics/PixelSettings.h"

GameObject& Renderer::addObject(GameObject&& gameObject)
{
    gameObjects.push_back(std::move(gameObject));
    return gameObjects.back();
}


void Renderer::render(Scene& scene, Camera& camera) const
{
    for (const GameObject& object : scene.getGameObjects())
    {
        const RenderComponent* renderComponent = object.getRenderComponent();

        if (renderComponent == nullptr || renderComponent -> mesh == nullptr || renderComponent -> material == nullptr)
        {
            continue;
        }

        const Material* material = renderComponent -> material;
        const ShaderProgram& shader = material -> getShader();

        shader.use();

        shader.setMat4("model", object.getTransform().getModelMatrix());
        shader.setMat4("view", camera.getViewMatrix());
        shader.setMat4("projection", camera.getProjectionMatrix());

        material->apply(camera, object);

        renderComponent->mesh->draw();
    }
}
