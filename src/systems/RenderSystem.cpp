#include <vector>

#include "systems/RenderSystem.h"
#include "components/RenderComponent.h"
#include "components/TransformComponent.h"

#include "graphics/materials/Material.h"
#include "graphics/ShaderProgram.h"

RenderSystem::RenderSystem(ComponentRegistry& registry) : registry_(registry)
{

}

void RenderSystem::render(const RenderView& view)
{
    std::vector<Entity> renderEntities = registry_.getEntitiesWith<RenderComponent, TransformComponent>();

    for (Entity entity : renderEntities)
    {
        RenderComponent* renderComponent = registry_.get<RenderComponent>(entity);
        TransformComponent* transformComponent = registry_.get<TransformComponent>(entity);

        if (renderComponent == nullptr || transformComponent == nullptr)
        {
            return;
        }

        if (renderComponent->material == nullptr || renderComponent->mesh == nullptr)
        {
            return;
        }

        const Material* material = renderComponent->material;
        const ShaderProgram& shader = material->getShader();

        shader.use();

        shader.setMat4("model", transformComponent->getModelMatrix());
        shader.setMat4("view", view.view);
        shader.setMat4("projection", view.projection);

        RenderContext context {view, *transformComponent};

        material->apply(context);

        renderComponent->mesh->draw();
    }
}