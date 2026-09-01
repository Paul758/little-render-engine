

#include "systems/InputSystem.h"
#include "components/PlayerInputComponent.h"
#include "Input.h"

InputSystem::InputSystem(ComponentRegistry& registry)
    : registry_(registry)
{

}

void InputSystem::update(IInput& input)
{
    std::vector<Entity> entities = registry_.getEntitiesWith<PlayerInputComponent>();

    for (Entity entity : entities)
    {
        Vec3 movementVector{0.0f, 0.0f, 0.0f};
        PlayerInputComponent* playerInputComponent = registry_.get<PlayerInputComponent>(entity);

        if (playerInputComponent == nullptr)
        {
            continue;
        }

        playerInputComponent->jumpPressed = false;
        playerInputComponent->throwPressed = false;

        if (input.isKeyDown(GLFW_KEY_W))
        {
            movementVector.z += 1.0f;
        }

        if (input.isKeyDown(GLFW_KEY_S))
        {
            movementVector.z -= 1.0f;
        }

        if (input.isKeyDown(GLFW_KEY_A))
        {
            movementVector.x -= 1.0f;
        }

        if (input.isKeyDown(GLFW_KEY_D))
        {
            movementVector.x += 1.0f;
        }

        playerInputComponent->jumpPressed = input.isKeyPressed(GLFW_KEY_SPACE);

        playerInputComponent->movement = movementVector;
    }
}