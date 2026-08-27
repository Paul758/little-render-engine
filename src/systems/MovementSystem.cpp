#include "MovementSystem.h"
#include "TransformComponent.h"
#include "VelocityComponent.h"

MovementSystem::MovementSystem(ComponentRegistry registry, GameTime time) : registry_(registry), time_(time)
{
    
}

void MovementSystem::update()
{
    registry_.each<TransformComponent, VelocityComponent>(
        [&](Entity, TransformComponent& transform, VelocityComponent& velocity)
    {
        transform.position += velocity.velocity * time_.deltaTime();
    });
}