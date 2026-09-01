#include <catch2/catch_test_macros.hpp>

#include "systems/InputSystem.h"
#include "ecs/World.h"
#include "components/PlayerInputComponent.h"
#include "fakes/FakeInput.h"

TEST_CASE("InputSystem writes forward movement")
{
    ComponentRegistry registry;
    Entity entity{0, 0};

    registry.add<PlayerInputComponent>(entity, PlayerInputComponent{});
    InputSystem inputSystem(registry);

    FakeInput input;
    input.wDown = true;

    inputSystem.update(input);

    PlayerInputComponent* playerInputComponent = registry.get<PlayerInputComponent>(entity);

    REQUIRE(playerInputComponent != nullptr);

    CHECK(playerInputComponent->movement.x == 0.0f);
    CHECK(playerInputComponent->movement.z == 1.0f);
}

TEST_CASE("InputSystem sets jump when jump key was pressed")
{
    ComponentRegistry registry;
    Entity entity{0, 0};

    registry.add<PlayerInputComponent>(entity, PlayerInputComponent{});
    InputSystem inputSystem(registry);

    FakeInput input;
    input.jumpPressed = true;

    inputSystem.update(input);

    PlayerInputComponent* playerInputComponent = registry.get<PlayerInputComponent>(entity);

    REQUIRE(playerInputComponent != nullptr);

    CHECK(playerInputComponent->jumpPressed == true);
}

TEST_CASE("InputSystem clears jump when jump is not pressed")
{
    ComponentRegistry registry;
    Entity entity{0, 0};

    PlayerInputComponent component;
    component.jumpPressed = true;

    registry.add<PlayerInputComponent>(entity, component);
    InputSystem inputSystem(registry);

    FakeInput input;
    input.jumpPressed = false;

    inputSystem.update(input);

    PlayerInputComponent* playerInputComponent = registry.get<PlayerInputComponent>(entity);

    REQUIRE(playerInputComponent != nullptr);

    CHECK_FALSE(playerInputComponent->jumpPressed);
}

TEST_CASE("Opposite movement keys cancel each other")
{
    ComponentRegistry registry;
    Entity entity{0, 0};

    registry.add<PlayerInputComponent>(entity, PlayerInputComponent{});

    InputSystem inputSystem(registry);

    FakeInput input;
    input.wDown = true;
    input.sDown = true;

    inputSystem.update(input);

    PlayerInputComponent* playerInput = registry.get<PlayerInputComponent>(entity);

    REQUIRE(playerInput != nullptr);

    CHECK(playerInput->movement.z == 0.0f);
}

TEST_CASE("Input system can produce diagonal input")
{
    ComponentRegistry registry;
    Entity entity{0, 0};

    registry.add<PlayerInputComponent>(entity, PlayerInputComponent{});

    InputSystem inputSystem(registry);

    FakeInput input;
    input.wDown = true;
    input.dDown = true;

    inputSystem.update(input);

    PlayerInputComponent* playerInput = registry.get<PlayerInputComponent>(entity);

    REQUIRE(playerInput != nullptr);

    CHECK(playerInput->movement.z == 1.0f);
    CHECK(playerInput->movement.x == 1.0f);
}