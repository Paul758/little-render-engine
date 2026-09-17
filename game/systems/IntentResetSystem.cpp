#include "game/systems/IntentResetSystem.h"
#include "game/components/LocomotionIntentComponent.h"

IntentResetSystem::IntentResetSystem(ComponentRegistry& registry)
    : componentRegistry(registry)
{

}

void IntentResetSystem::update()
{
    auto entities = componentRegistry.getEntitiesWith<LocomotionIntentComponent>();

    for (Entity entity : entities)
    {
        auto* intent = componentRegistry.get<LocomotionIntentComponent>(entity);
        *intent = LocomotionIntentComponent{};
    }
}