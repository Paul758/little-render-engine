#include <vector>

#include "engine/core/ecs/Entity.h"
#include "engine/core/ecs/ComponentRegistry.h"

#include "engine/components/lighting/DirectionalLightComponent.h"
#include "engine/components/lighting/LightingData.h"
#include "engine/components/lighting/DirectionalLightData.h"
#include "engine/components/TransformComponent.h"
#include "engine/components/TransformUtils.h"

#include "engine/systems/LightingSystem.h"


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