#include <assert.h>
#include <stdexcept>

#include "systems/CameraSystem.h"

#include "components/TransformComponent.h"
#include "components/camera/CameraComponent.h"
#include "components/TransformUtils.h"
#include "graphics/RenderViewport.h"


CameraSystem::CameraSystem(ComponentRegistry& registry)
    : registry_(registry)
{

}

RenderView CameraSystem::buildRenderView(const RenderViewport& viewport)
{
    Entity camera = viewport.camera;

    TransformComponent* cameraTransform = registry_.get<TransformComponent>(camera);
    CameraComponent* cameraComponent = registry_.get<CameraComponent>(camera);

    if (cameraTransform == nullptr)
    {
        throw std::runtime_error("Camera entity has no TransformComponent");
    }

    if (cameraComponent == nullptr)
    {
        throw std::runtime_error("Camera entity has no CameraComponent");
    }
    
    CameraBasis cameraBasis = buildCameraBasis(*cameraTransform);

    if (viewport.height <= 0)
    {
        throw std::invalid_argument("Viewport height must be greater than zero");
    }

    const float aspectRatio = 
        static_cast<float>(viewport.width) /
        static_cast<float>(viewport.height);

    Mat4 view = Mat4::view(cameraTransform->position, cameraBasis.forward, cameraBasis.right, cameraBasis.up);
    Mat4 projection = buildProjectionMatrix(*cameraComponent, aspectRatio);

    return RenderView{view, projection, cameraBasis};
}

CameraBasis CameraSystem::buildCameraBasis(const TransformComponent& transform) const
{
    return CameraBasis{
        TransformUtils::getForward(transform),
        TransformUtils::getRight(transform),
        TransformUtils::getUp(transform)
    };
}

Mat4 CameraSystem::buildProjectionMatrix(const CameraComponent& camera, float aspectRatio) const
{
    switch(camera.projectionType)
    {
        case ProjectionType::Perspective:
            return Mat4::perspective(camera.fieldOfView, aspectRatio, camera.nearPlane, camera.farPlane);

        case ProjectionType::Orthographic:
        {
            float halfHeight = camera.orthographicSize / 2.0f;
            float halfWidth = halfHeight * aspectRatio;
            return Mat4::orthographic(-halfWidth, halfWidth, -halfHeight, halfHeight, camera.nearPlane, camera.farPlane);
        }
            
    }
    throw std::runtime_error("Unknown camera projection type");
}
