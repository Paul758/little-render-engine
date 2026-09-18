#include <vector>


#include "engine/math/MathUtils.h"
#include "engine/components/TransformComponent.h"

#include "engine/components/camera/WorldCameraComponent.h"
#include "game/components/camera/OrbitPose.h"
#include "game/components/camera/FixedOrbitCameraComponent.h"
#include "game/systems/camera/FixedOrbitCameraSystem.h"

FixedOrbitCameraSystem::FixedOrbitCameraSystem(ComponentRegistry& registry)
    : registry_(registry)
{

}

void FixedOrbitCameraSystem::update(const Input& input, Entity activeCameraEntity, float deltaTime)
{
    TransformComponent* transform = registry_.get<TransformComponent>(activeCameraEntity);
    FixedOrbitCameraComponent* fixedOrbitCameraComponent = registry_.get<FixedOrbitCameraComponent>(activeCameraEntity);
    WorldCameraComponent* worldCameraComponent = registry_.get<WorldCameraComponent>(activeCameraEntity);

    if (transform == nullptr || fixedOrbitCameraComponent == nullptr || worldCameraComponent == nullptr)
    {
        return;
    }

    size_t selectedPose = fixedOrbitCameraComponent->selectedPose;
    std::vector<OrbitPose>& orbitPoses = fixedOrbitCameraComponent->poses;

    const bool rotating = std::abs(orbitPoses[selectedPose].poseYaw - worldCameraComponent->yaw) > 0.01f;

    if (!rotating)
    {
        if (input.isKeyPressed(GLFW_KEY_Q))
        {
            if (selectedPose == 0)
            {
                selectedPose = fixedOrbitCameraComponent->poses.size() - 1;
            }
            else
            {
                selectedPose--;
            }

            fixedOrbitCameraComponent->selectedPose = selectedPose;
        }

        if (input.isKeyPressed(GLFW_KEY_E))
        {
            selectedPose = (selectedPose + 1) % fixedOrbitCameraComponent->poses.size();
            fixedOrbitCameraComponent->selectedPose = selectedPose;
        }
    }

    const float blend = 1.0f - std::exp(-fixedOrbitCameraComponent->rotationSpeed * deltaTime);

    float currentYaw = worldCameraComponent->yaw;
    float targetYaw = fixedOrbitCameraComponent->poses[fixedOrbitCameraComponent->selectedPose].poseYaw;

    worldCameraComponent->yaw += (targetYaw - worldCameraComponent->yaw) * blend;
    
    updateCameraTransform(transform, worldCameraComponent, fixedOrbitCameraComponent);
}

void FixedOrbitCameraSystem::initialize(Entity activeCameraEntity)
{
    TransformComponent* transform = registry_.get<TransformComponent>(activeCameraEntity);
    FixedOrbitCameraComponent* fixedOrbitCameraComponent = registry_.get<FixedOrbitCameraComponent>(activeCameraEntity);
    WorldCameraComponent* worldCameraComponent = registry_.get<WorldCameraComponent>(activeCameraEntity);

    if (transform == nullptr || fixedOrbitCameraComponent == nullptr || worldCameraComponent == nullptr)
    {
        return;
    }

    if (fixedOrbitCameraComponent->poses.empty())
    {
        return;
    }

    if (fixedOrbitCameraComponent->selectedPose >= fixedOrbitCameraComponent->poses.size())
    {
        fixedOrbitCameraComponent->selectedPose = 0;
    }

    worldCameraComponent->yaw = fixedOrbitCameraComponent->poses[fixedOrbitCameraComponent->selectedPose].poseYaw;
    worldCameraComponent->pitch = fixedOrbitCameraComponent->poses[fixedOrbitCameraComponent->selectedPose].posePitch;

    updateCameraTransform(transform, worldCameraComponent, fixedOrbitCameraComponent);
}

void FixedOrbitCameraSystem::updateCameraTransform(TransformComponent* transform, WorldCameraComponent* worldCamera, FixedOrbitCameraComponent* fixedOrbit)
{
    const float yawRadians = Math::radians(worldCamera->yaw);
    const float pitchRadians = Math::radians(worldCamera->pitch);

    const float radius = fixedOrbit->radius;

    const Vec3 offset {
        radius * std::cos(pitchRadians) * std::sin(yawRadians),
        radius * std::sin(pitchRadians),
        radius * std::cos(pitchRadians) * std::cos(yawRadians)
    };

    transform->position = fixedOrbit->target + offset;

    Vec3 forward = (fixedOrbit->target - transform->position).normalized();

    transform->rotation = Quaternion::lookRotation(forward, Vec3{0.0f, 1.0f, 0.0f});
}