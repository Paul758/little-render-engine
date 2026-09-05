#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include "ecs/World.h"
#include "ecs/Entity.h"

#include "components/PlayerInputComponent.h"
#include "components/LocomotionIntentComponent.h"

#include "systems/IntentResetSystem.h"


TEST_CASE("IntentResetSystem resets a LocomotionIntent")
{
    World world;
    Entity entityFirst = world.createEntity();
    Entity entitySecond = world.createEntity();

    LocomotionIntentComponent componentFirst{Vec3{1.0f, 2.0f, 3.0f}, true};
    LocomotionIntentComponent componentSecond{Vec3{4.0f, 5.0f, 6.0f}, true};

    world.components().add<LocomotionIntentComponent>(entityFirst, componentFirst);
    world.components().add<LocomotionIntentComponent>(entitySecond, componentSecond);

    IntentResetSystem intentResetSystem(world.components());

    intentResetSystem.update();

    LocomotionIntentComponent* componentRetrievedFirst = world.components().get<LocomotionIntentComponent>(entityFirst);
    LocomotionIntentComponent* componentRetrievedSecond = world.components().get<LocomotionIntentComponent>(entitySecond);

    REQUIRE(componentRetrievedFirst != nullptr);
    REQUIRE(componentRetrievedSecond != nullptr);

    REQUIRE(componentRetrievedFirst->movementDirection == Vec3{0.0f, 0.0f, 0.0f});
    REQUIRE(componentRetrievedFirst->wantsToJump == false);

    REQUIRE(componentRetrievedSecond->movementDirection == Vec3{0.0f, 0.0f, 0.0f});
    REQUIRE(componentRetrievedSecond->wantsToJump == false);
}

TEST_CASE("IntentResetSystem ignores entities without LocomotionIntentComponent")
{
    World world;
    Entity entity = world.createEntity();

    world.components().add<PlayerInputComponent>(entity, PlayerInputComponent{});

    IntentResetSystem system(world.components());

    REQUIRE_NOTHROW(system.update());

    REQUIRE(world.components().get<LocomotionIntentComponent>(entity) == nullptr);
}