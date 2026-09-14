#include <vector>

#include "systems/RenderSystem.h"
#include "components/RenderComponent.h"
#include "components/TransformComponent.h"
#include "components/lighting/LightingData.h"


#include "graphics/materials/Material.h"
#include "graphics/ShaderProgram.h"

RenderSystem::RenderSystem(ComponentRegistry& registry) : registry_(registry)
{

}

void RenderSystem::render(const RenderView& view, const LightingData& lightingData)
{
    std::vector<Entity> renderEntities = registry_.getEntitiesWith<RenderComponent, TransformComponent>();

    for (Entity entity : renderEntities)
    {
        RenderComponent* renderComponent = registry_.get<RenderComponent>(entity);
        TransformComponent* transformComponent = registry_.get<TransformComponent>(entity);

        if (renderComponent == nullptr || transformComponent == nullptr)
        {
            continue;
        }

        if (renderComponent->material == nullptr || renderComponent->mesh == nullptr)
        {
            continue;
        }

        const Material* material = renderComponent->material;
        const ShaderProgram& shader = material->getShader();

        shader.use();

        // View
        shader.setMat4("model", transformComponent->getModelMatrix());
        shader.setMat4("view", view.view);
        shader.setMat4("projection", view.projection);

        RenderContext context {view, *transformComponent, lightingData};

        material->apply(context);

        renderComponent->mesh->draw();
    }
}