#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include "engine/core/ecs/World.h"
#include "engine/core/ecs/Entity.h"
#include "engine/math/Mat4.h"
#include "engine/components/TransformComponent.h"
#include "engine/systems/HierarchySystem.h"
#include "engine/systems/TransformSystem.h"

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

TEST_CASE("TransformSystem calculates world transform for root entity")
{
    World world;
    Entity entity = world.createEntity();

    TransformComponent transform;
    transform.position = Vec3{10.0f, 2.0f, 3.0f};

    world.components().add<TransformComponent>(entity, transform);

    HierarchySystem hierarchySystem(world.components());

    TransformSystem transformSystem(world.components(), hierarchySystem);

    transformSystem.update();

    const TransformComponent* result = world.components().get<TransformComponent>(entity);

    REQUIRE(result != nullptr);

    requireMat4ApproxEqual(result->getWorldMatrix(), result->getLocalMatrix());
}

TEST_CASE("TransformSystem propagates parent translation to child")
{
    World world;

    Entity parent = world.createEntity();
    Entity child = world.createEntity();

    TransformComponent parentTransform;
    parentTransform.position = Vec3{10.0f, 0.0f, 0.0f};

    TransformComponent childTransform;
    childTransform.position = Vec3{2.0f, 0.0f, 0.0f};

    world.components().add<TransformComponent>(parent, parentTransform);
    world.components().add<TransformComponent>(child, childTransform);

    HierarchySystem hierarchySystem(world.components());
    hierarchySystem.setParent(child, parent);

    TransformSystem transformSystem(world.components(), hierarchySystem);

    transformSystem.update();

    const TransformComponent* result = world.components().get<TransformComponent>(child);

    REQUIRE(result != nullptr);

    Mat4 expected = parentTransform.getLocalMatrix()
                    * childTransform.getLocalMatrix();
    
    requireMat4ApproxEqual(result->getWorldMatrix(), expected);
}

TEST_CASE("TransformSystem propagates transforms through multiple hierarchy levels")
{
    World world;

    Entity parent = world.createEntity();
    Entity child = world.createEntity();
    Entity grandChild = world.createEntity();

    TransformComponent parentTransform;
    parentTransform.position = Vec3{10.0f, 0.0f, 0.0f};

    TransformComponent childTransform;
    childTransform.position = Vec3{2.0f, 0.0f, 0.0f};

    TransformComponent grandChildTransform;
    grandChildTransform.position = Vec3{3.0f, 0.0f, 0.0f};

    world.components().add<TransformComponent>(parent, parentTransform);
    world.components().add<TransformComponent>(child, childTransform);
    world.components().add<TransformComponent>(grandChild, grandChildTransform);

    HierarchySystem hierarchySystem(world.components());
    hierarchySystem.setParent(child, parent);
    hierarchySystem.setParent(grandChild, child);

    TransformSystem transformSystem(world.components(), hierarchySystem);

    transformSystem.update();

    const TransformComponent* result = world.components().get<TransformComponent>(grandChild);

    REQUIRE(result != nullptr);

    Mat4 expected = parentTransform.getLocalMatrix()
                    * childTransform.getLocalMatrix()
                    * grandChildTransform.getLocalMatrix();
    
    requireMat4ApproxEqual(result->getWorldMatrix(), expected);
}

TEST_CASE("TransformSystem applies parent rotation to child position")
{
    World world;

    Entity parent = world.createEntity();
    Entity child = world.createEntity();

    TransformComponent parentTransform;
    parentTransform.rotation = Quaternion::fromAxisAngle(Vec3{0.0f, 1.0f, 0.0f}, 90.0f);

    TransformComponent childTransform;
    childTransform.position = Vec3{0.0f, 0.0f, -1.0f};

    world.components().add<TransformComponent>(parent, parentTransform);
    world.components().add<TransformComponent>(child, childTransform);

    HierarchySystem hierarchySystem(world.components());
    hierarchySystem.setParent(child, parent);

    TransformSystem transformSystem(world.components(), hierarchySystem);

    transformSystem.update();

    const TransformComponent* result = world.components().get<TransformComponent>(child);

    REQUIRE(result != nullptr);

    const Vec3 worldPosition = result->getWorldMatrix().transformPoint(Vec3{0.0f, 0.0f, 0.0f});
    
    REQUIRE(worldPosition.x == Catch::Approx(-1.0f).margin(0.00001f));
    REQUIRE(worldPosition.y == Catch::Approx(0.0f).margin(0.00001f));
    REQUIRE(worldPosition.z == Catch::Approx(0.0f).margin(0.00001f));
}

TEST_CASE("TransformSystem applies parent scale to child position")
{
    World world;

    Entity parent = world.createEntity();
    Entity child = world.createEntity();

    TransformComponent parentTransform;
    parentTransform.scale = Vec3{2.0f, 2.0f, 2.0f};

    TransformComponent childTransform;
    childTransform.position = Vec3{1.0f, 0.0f, 0.0f};

    world.components().add<TransformComponent>(parent, parentTransform);
    world.components().add<TransformComponent>(child, childTransform);

    HierarchySystem hierarchySystem(world.components());
    hierarchySystem.setParent(child, parent);

    TransformSystem transformSystem(world.components(), hierarchySystem);

    transformSystem.update();

    const TransformComponent* result = world.components().get<TransformComponent>(child);

    REQUIRE(result != nullptr);

    const Vec3 worldPosition = result->getWorldMatrix().transformPoint(Vec3{0.0f, 0.0f, 0.0f});

    REQUIRE(worldPosition.x == Catch::Approx(2.0f).margin(0.00001f));
    REQUIRE(worldPosition.y == Catch::Approx(0.0f).margin(0.00001f));
    REQUIRE(worldPosition.z == Catch::Approx(0.0f).margin(0.00001f));
}