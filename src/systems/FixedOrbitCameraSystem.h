

#include "ecs/Entity.h"
#include "ecs/ComponentRegistry.h"

#include "components/camera/FixedOrbitCameraComponent.h"
#include "components/camera/WorldCameraComponent.h"
#include "components/TransformComponent.h"
#include "Input.h"

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