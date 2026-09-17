#include "PlayerMovementBTreeBuilder.h"

#include "engine/behaviour/BehaviourTree.h"
#include "engine/behaviour/composites/EvaluateAll.h"
#include "engine/behaviour/composites/Sequence.h"
#include "game/behaviour/player/PlayerMovementNodes.h"

std::unique_ptr<BehaviourTree> PlayerMovementBTreeBuilder::build()
{
    auto root = std::make_unique<EvaluateAll>();

    auto movementSequence = std::make_unique<Sequence>();
    movementSequence->addChild(std::make_unique<CheckHasInputComponent>());
    movementSequence->addChild(std::make_unique<TaskSetMovementIntent>());
    root->addChild(std::move(movementSequence));

    auto jumpSequence = std::make_unique<Sequence>();
    jumpSequence->addChild(std::make_unique<CheckHasJumpInput>());
    jumpSequence->addChild(std::make_unique<TaskSetJumpIntent>());
    root->addChild(std::move(jumpSequence));

    return std::make_unique<BehaviourTree>(std::move(root));
}