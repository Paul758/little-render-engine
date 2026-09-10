#include <assert.h>

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

    assert(cameraTransform != nullptr);
    assert(cameraComponent != nullptr);
    
    CameraBasis cameraBasis = buildCameraBasis(*cameraTransform);

    assert(viewport.height != 0.0f);

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
}
