#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include "ecs/World.h"
#include "ecs/Entity.h"

#include "systems/CameraSystem.h"

#include "components/camera/CameraComponent.h"
#include "components/TransformComponent.h"

#include "graphics/RenderViewport.h"
#include "graphics/RenderView.h"

TEST_CASE("CameraSystem builds a RenderView")
{
    World world;
    Entity camera = world.createEntity();

    world.components().add<CameraComponent>(camera, CameraComponent{});
    world.components().add<TransformComponent>(camera, TransformComponent{});

    CameraSystem cameraSystem(world.components());
    RenderViewport renderViewport{camera, 800, 600};

    RenderView renderView = cameraSystem.buildRenderView(renderViewport);

    
}