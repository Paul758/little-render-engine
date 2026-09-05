#include "systems/IntentResetSystem.h"
#include "components/LocomotionIntentComponent.h"

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