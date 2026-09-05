#include "Integrationsystem.h"
#include "components/TransformComponent.h"
#include "components/VelocityComponent.h"

IntegrationSystem::IntegrationSystem(ComponentRegistry& registry) : registry_(registry)
{
    
}

void IntegrationSystem::update(float deltaTime)
{
    std::vector<Entity> entities = registry_.getEntitiesWith<TransformComponent, VelocityComponent>();

    for (Entity entity : entities) {

        TransformComponent* transform = registry_.get<TransformComponent>(entity);
        VelocityComponent* velocity = registry_.get<VelocityComponent>(entity);

        if (transform == nullptr || velocity == nullptr)
        {
            continue;
        }

        transform->position += velocity->velocity * deltaTime;
    }   

}