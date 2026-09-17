
#include "engine/core/ecs/Entity.h"
#include "engine/core/ecs/ComponentRegistry.h"
#include "engine/components/camera/WorldCameraComponent.h"
#include "engine/components/TransformComponent.h"
#include "engine/core/input/Input.h"

#include "game/components/camera/FixedOrbitCameraComponent.h"

class FixedOrbitCameraSystem
{
public:
    FixedOrbitCameraSystem(ComponentRegistry& componentRegistry);
    void update(Input& input, Entity cameraEntity, float deltaTime);
    void initialize(Entity cameraEntity);
    
private:
    void updateCameraTransform(
        TransformComponent* transformComponent,
        WorldCameraComponent* worldCameraComponent,
        FixedOrbitCameraComponent* fixedOrbit
    );
private:
    ComponentRegistry& registry_;
};