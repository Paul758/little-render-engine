#pragma once

#include "behaviour/Node.h"
#include "components/PlayerInputComponent.h"
#include "components/LocomotionIntentComponent.h"

class CheckHasInputComponent : public Node
{
public:
    NodeState update(BehaviourContext& context) override
    {
        PlayerInputComponent* playerInputComponent = context.get<PlayerInputComponent>();

        if (playerInputComponent == nullptr)
        {
            return NodeState::FAILURE;
        }
        return NodeState::SUCCESS;
    }
};

class TaskSetMovementIntent : public Node
{
public:
    NodeState update(BehaviourContext& context) override
    {
        PlayerInputComponent* playerInputComponent = context.get<PlayerInputComponent>();
        LocomotionIntentComponent* locomotionIntentComponent = context.get<LocomotionIntentComponent>();

        if (playerInputComponent == nullptr || locomotionIntentComponent == nullptr)
        {
            return NodeState::FAILURE;
        }

        locomotionIntentComponent->movementDirection = playerInputComponent->movement;
        return NodeState::SUCCESS;
    }
};


class CheckHasJumpInput : public Node
{
    NodeState update(BehaviourContext& context) override
    {
        PlayerInputComponent* playerInputComponent = context.get<PlayerInputComponent>();

        if (playerInputComponent == nullptr)
        {
            return NodeState::FAILURE;
        }

        if (!playerInputComponent->jumpPressed)
        {
            return NodeState::FAILURE;
        }

        return NodeState::SUCCESS;
    }
};

class TaskSetJumpIntent : public Node
{
    NodeState update(BehaviourContext& context) override
    {
        PlayerInputComponent* playerInputComponent = context.get<PlayerInputComponent>();
        LocomotionIntentComponent* locomotionIntentComponent = context.get<LocomotionIntentComponent>();

        if (playerInputComponent == nullptr || locomotionIntentComponent == nullptr)
        {
            return NodeState::FAILURE;
        }

        locomotionIntentComponent->wantsToJump = true;

        return NodeState::SUCCESS;
    }
};
