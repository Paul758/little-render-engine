#pragma once

#include "components/camera/CameraInputComponent.h"
#include "components/camera/CameraIntentComponent.h"
#include "behaviour/Node.h"

class TaskToggleFreeFlyCameraComponent : public Node
{
public:
    NodeState update(BehaviourContext& context) override
    {
        CameraInputComponent* cameraInputComponent = context.get<CameraInputComponent>();
        CameraIntentComponent* cameraIntentComponent = context.get<CameraIntentComponent>();

        if (cameraInputComponent == nullptr || cameraIntentComponent == nullptr)
        {
            return NodeState::FAILURE;
        }

        if (cameraInputComponent->toggleFreeFlyPressed)
        {
            return NodeState::FAILURE;
        }

        if(context.componentRegistry.has<FreeFlyCameraComponent>(context.entity))
        {
            cameraIntentComponent->requests.push_back(CameraComponentRequest{CameraBehaviourType::FreeFly, CameraRequestAction::Remove});
        }
        else 
        {
            cameraIntentComponent->requests.push_back(CameraComponentRequest{CameraBehaviourType::FreeFly, CameraRequestAction::Add});
        }
        
    }
};

class TaskToggleOrbitCameraComponent : public Node
{
public:
    NodeState update(BehaviourContext& context) override
    {
        CameraInputComponent* cameraInputComponent = context.get<CameraInputComponent>();
        CameraIntentComponent* cameraIntentComponent = context.get<CameraIntentComponent>();

        if (cameraInputComponent == nullptr || cameraIntentComponent == nullptr)
        {
            return NodeState::FAILURE;
        }

        if (!cameraInputComponent->toggleOrbitPressed)
        {
            return NodeState::FAILURE;
        }

        if(context.componentRegistry.has<OrbitCameraComponent>(context.entity))
        {
            cameraIntentComponent->requests.push_back(CameraComponentRequest{CameraBehaviourType::Orbit, CameraRequestAction::Remove});
        }
        else 
        {
            cameraIntentComponent->requests.push_back(CameraComponentRequest{CameraBehaviourType::Orbit, CameraRequestAction::Add});
        }
    }
        
};

class TaskToggleFollowCameraComponent : public Node
{
public:
    NodeState update(BehaviourContext& context) override
    {
        CameraInputComponent* cameraInputComponent = context.get<CameraInputComponent>();
        CameraIntentComponent* cameraIntentComponent = context.get<CameraIntentComponent>();

        if (cameraInputComponent == nullptr || cameraIntentComponent == nullptr)
        {
            return NodeState::FAILURE;
        }

        if (!cameraInputComponent->toggleFollowPressed)
        {
            return NodeState::FAILURE;
        }

        if(context.componentRegistry.has<FollowCameraComponent>(context.entity))
        {
            cameraIntentComponent->requests.push_back(CameraComponentRequest{CameraBehaviourType::Follow, CameraRequestAction::Remove});
        }
        else 
        {
            cameraIntentComponent->requests.push_back(CameraComponentRequest{CameraBehaviourType::Follow, CameraRequestAction::Add});
        }
    }
};