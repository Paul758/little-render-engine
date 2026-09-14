#include <vector>

#include "ecs/Entity.h"
#include "ecs/ComponentRegistry.h"

#include "components/lighting/DirectionalLightComponent.h"
#include "components/lighting/LightingData.h"
#include "components/lighting/DirectionalLightData.h"
#include "components/TransformComponent.h"
#include "components/TransformUtils.h"

#include "systems/LightingSystem.h"


LightingSystem::LightingSystem(ComponentRegistry& registry)
    : registry_(registry)
{

}

LightingData LightingSystem::buildLightingData()
{
    std::vector<Entity> lightEntities = registry_.getEntitiesWith<TransformComponent, DirectionalLightComponent>();
    LightingData result;

    for(Entity lightEntity : lightEntities)
    {
        TransformComponent* transform = registry_.get<TransformComponent>(lightEntity);
        DirectionalLightComponent* directionalLight = registry_.get<DirectionalLightComponent>(lightEntity);

        if (transform == nullptr || directionalLight == nullptr)
        {
            continue;
        }

        result.directionalLights.push_back(
            DirectionalLightData{
                .direction = TransformUtils::getForward(*transform),
                .color = directionalLight->color,
                .intensity = directionalLight->intensity
            }
        );

    }

    return result;
}