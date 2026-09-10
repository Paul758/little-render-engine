#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include "ecs/World.h"
#include "ecs/Entity.h"

#include "systems/CameraSystem.h"

#include "components/camera/CameraComponent.h"
#include "components/TransformComponent.h"
#include "components/TransformUtils.h"

#include "graphics/RenderViewport.h"
#include "graphics/RenderView.h"

#include "math/MathUtils.h"

using Catch::Approx;

inline void requireMat4ApproxEqual(const Mat4& actual, const Mat4& expected, float epsilon = 0.00001f)
{
    for (std::size_t row = 0; row < 4; ++row)
    {
        for (std::size_t column = 0; column < 4; ++column)
        {
            REQUIRE(actual.at(row, column) == Approx(expected.at(row, column)).margin(epsilon));
        }
    }
}

TEST_CASE("CameraSystem builds a RenderView")
{
    World world;
    Entity camera = world.createEntity();

    world.components().add<CameraComponent>(camera, CameraComponent{});
    world.components().add<TransformComponent>(camera, TransformComponent{});

    CameraSystem cameraSystem(world.components());
    RenderViewport renderViewport{camera, 800.0f, 600.0f};
    
    float aspectRatio = static_cast<float>(renderViewport.width)
                         / static_cast<float>(renderViewport.height); 

    RenderView renderView = cameraSystem.buildRenderView(renderViewport);

    TransformComponent* retrievedTransform = world.components().get<TransformComponent>(camera);
    
    Vec3 forward = TransformUtils::getForward(*retrievedTransform);
    Vec3 right = TransformUtils::getRight(*retrievedTransform);
    Vec3 up = TransformUtils::getUp(*retrievedTransform);

    Mat4 viewMatrix = Mat4::view(
        retrievedTransform->position,
        forward,
        right,
        up
    );

    CameraComponent* cameraComponent = world.components().get<CameraComponent>(camera);

    Mat4 projectionMatrix = Mat4::perspective(
        cameraComponent->fieldOfView,
        aspectRatio,
        cameraComponent->nearPlane,
        cameraComponent->farPlane
    );

    requireMat4ApproxEqual(viewMatrix, renderView.view);
    requireMat4ApproxEqual(projectionMatrix, renderView.projection);
    REQUIRE(renderView.cameraBasis.forward == forward);
    REQUIRE(renderView.cameraBasis.right == right);
    REQUIRE(renderView.cameraBasis.up == up);


}

TEST_CASE("Camera System builds a perspective render view")
{
    World world;
    Entity camera = world.createEntity();

    CameraComponent cameraComponent{};
    cameraComponent.projectionType = ProjectionType::Perspective;
    cameraComponent.fieldOfView = Math::radians(60.0f);
    cameraComponent.nearPlane = 0.1f;
    cameraComponent.farPlane = 1000.0f;

    TransformComponent transform{};
    transform.position = Vec3{3.0f, 2.0f, 5.0f};
    transform.rotation = Quaternion::fromEuler(Vec3{10.0f, 30.0f, 0.0f});

    world.components().add<CameraComponent>(camera, cameraComponent);
    world.components().add<TransformComponent>(camera, transform);

    CameraSystem cameraSystem(world.components());

    RenderViewport viewport{camera, 800, 600};

    RenderView actual = cameraSystem.buildRenderView(viewport);

    Vec3 forward = TransformUtils::getForward(transform);
    Vec3 right = TransformUtils::getRight(transform);
    Vec3 up = TransformUtils::getUp(transform);

    Mat4 expectedView = Mat4::view(
        transform.position,
        forward,
        right,
        up
    );

    float aspectRatio = static_cast<float>(viewport.width)
                     / static_cast<float>(viewport.height); 

    Mat4 expectedProjection = Mat4::perspective(
        cameraComponent.fieldOfView,
        aspectRatio,
        cameraComponent.nearPlane,
        cameraComponent.farPlane
    );

    requireMat4ApproxEqual(actual.view, expectedView);

    requireMat4ApproxEqual(actual.projection, expectedProjection);

    REQUIRE(actual.cameraBasis.forward == forward);
    REQUIRE(actual.cameraBasis.right == right);
    REQUIRE(actual.cameraBasis.up == up);
}

TEST_CASE("CameraSystem produces correct basis for 90 degree Y rotation")
{
    World world;
    Entity camera = world.createEntity();

    world.components().add<CameraComponent>(camera, CameraComponent{});

    TransformComponent transform{};
    transform.rotation = Quaternion::fromAxisAngle(Vec3{0.0f, 1.0f, 0.0f}, 90.0f);

    world.components().add<TransformComponent>(camera, transform);

    CameraSystem cameraSystem(world.components());

    RenderViewport viewport{camera, 800, 600};

    RenderView view = cameraSystem.buildRenderView(viewport);

    REQUIRE(view.cameraBasis.forward.x == Approx(-1.0f).margin(0.00001f));
    REQUIRE(view.cameraBasis.forward.y == Approx(0.0f).margin(0.00001f));
    REQUIRE(view.cameraBasis.forward.z == Approx(0.0f).margin(0.00001f));
}

TEST_CASE("CameraSystem builds orthographic projection")
{
    World world;
    Entity camera = world.createEntity();

    CameraComponent cameraComponent{};
    cameraComponent.projectionType = ProjectionType::Orthographic;

    cameraComponent.orthographicSize = 10.0f;
    cameraComponent.nearPlane = 0.1f;
    cameraComponent.farPlane = 100.0f;

    world.components().add<CameraComponent>(camera, cameraComponent);

    world.components().add<TransformComponent>(camera, TransformComponent{});

    CameraSystem cameraSystem(world.components());

    RenderViewport viewport{camera, 800, 600};

    RenderView actual = cameraSystem.buildRenderView(viewport);

    float aspectRatio = static_cast<float>(viewport.width)
                     / static_cast<float>(viewport.height); 

    float halfHeight = cameraComponent.orthographicSize / 2.0f;
    float halfWidth = halfHeight * aspectRatio;

    Mat4 expected = Mat4::orthographic(
        -halfWidth,
        halfWidth,
        -halfHeight,
        halfHeight,
        cameraComponent.nearPlane,
        cameraComponent.farPlane
    );

    requireMat4ApproxEqual(actual.projection, expected);
}

TEST_CASE("CameraSystem produces orthonormal camera basis")
{
    World world;
    Entity camera = world.createEntity();

    TransformComponent transform{};
    transform.rotation = Quaternion::fromEuler(Vec3{20.0f, 30.0f, 40.0f});

    world.components().add<CameraComponent>(camera, CameraComponent{});

    world.components().add<TransformComponent>(camera, transform);

    CameraSystem cameraSystem(world.components());

    RenderView view = cameraSystem.buildRenderView(RenderViewport{camera, 800, 600});

    REQUIRE(view.cameraBasis.forward.length() == Approx(1.0f).margin(0.00001f));
    REQUIRE(view.cameraBasis.right.length() == Approx(1.0f).margin(0.00001f));
    REQUIRE(view.cameraBasis.up.length() == Approx(1.0f).margin(0.00001f));

    REQUIRE(Vec3::dot(
            view.cameraBasis.forward,
            view.cameraBasis.right) ==
            Approx(0.0f).margin(0.00001f));

    REQUIRE(Vec3::dot(
            view.cameraBasis.forward,
            view.cameraBasis.up) ==
            Approx(0.0f).margin(0.00001f));
    
    REQUIRE(Vec3::dot(
            view.cameraBasis.right,
            view.cameraBasis.up) ==
            Approx(0.0f).margin(0.00001f));
}

TEST_CASE("CameraSystem projection changes with viewport aspect ratio")
{
    World world;
    Entity camera = world.createEntity();

    world.components().add<CameraComponent>(camera, CameraComponent{});

    world.components().add<TransformComponent>(camera, TransformComponent{});

    CameraSystem cameraSystem(world.components());

    RenderView square = cameraSystem.buildRenderView(RenderViewport{camera, 600, 600});

    RenderView wide = cameraSystem.buildRenderView(RenderViewport{camera, 1200, 600});

    REQUIRE(square.projection.at(0, 0) != Approx(wide.projection.at(0, 0)));
}

TEST_CASE("CameraSystem rejects zero viewport height")
{
    World world;
    Entity camera = world.createEntity();

    world.components().add<CameraComponent>(camera, CameraComponent{});

    world.components().add<TransformComponent>(camera, TransformComponent{});

    CameraSystem cameraSystem(world.components());

    REQUIRE_THROWS_AS(cameraSystem.buildRenderView(RenderViewport{camera, 800, 0}), std::invalid_argument);
}

TEST_CASE("CameraSystem rejects entity without TransformComponent")
{
    World world;
    Entity camera = world.createEntity();

    world.components().add<CameraComponent>(camera, CameraComponent{});

    CameraSystem cameraSystem(world.components());

    REQUIRE_THROWS_AS(cameraSystem.buildRenderView(RenderViewport{camera, 800, 0}), std::runtime_error);
}

TEST_CASE("CameraSystem rejects entity without CameraComponent")
{
    World world;
    Entity camera = world.createEntity();

    world.components().add<TransformComponent>(camera, TransformComponent{});

    CameraSystem cameraSystem(world.components());

    REQUIRE_THROWS_AS(cameraSystem.buildRenderView(RenderViewport{camera, 800, 0}), std::runtime_error);
}

TEST_CASE("CameraSystem view transforms camera position to origin")
{
    World world;
    Entity camera = world.createEntity();
    
    TransformComponent transform{};
    transform.position = Vec3{3.0f, 2.0f, 5.0f};

    world.components().add<TransformComponent>(camera, transform);
    world.components().add<CameraComponent>(camera, CameraComponent{});

    CameraSystem cameraSystem(world.components());

    RenderView renderView = cameraSystem.buildRenderView(RenderViewport{camera, 800, 600});

    Vec3 cameraInViewSpace = renderView.view.transformPoint(transform.position);

    REQUIRE(cameraInViewSpace.x == Approx(0.0f));
    REQUIRE(cameraInViewSpace.y == Approx(0.0f));
    REQUIRE(cameraInViewSpace.z == Approx(0.0f));
}
