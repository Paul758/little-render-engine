#pragma once
#include "engine/behaviour/BehaviourTree.h"

class PlayerMovementBTreeBuilder
{
public:
    std::unique_ptr<BehaviourTree> build();
};