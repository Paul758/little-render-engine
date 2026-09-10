#pragma once

#include "behaviour/BehaviourTree.h"
#include "behaviour/composites/EvaluateAll.h"
#include "behaviour/camera/CameraSwitchNodes.h"

class CameraComponentBTreeBuilder
{
public:
    std::unique_ptr<BehaviourTree> build()
    {
        auto root = std::make_unique<EvaluateAll>();

        root->addChild(std::make_unique<TaskToggleFreeFlyCameraComponent>());
        root->addChild(std::make_unique<TaskToggleOrbitCameraComponent>());
        root->addChild(std::make_unique<TaskToggleFollowCameraComponent>());

        return std::make_unique<BehaviourTree>(std::move(root));
    }
};