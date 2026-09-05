#pragma once
#include "behaviour/BehaviourTree.h"

class PlayerMovementBTreeBuilder
{
public:
    std::unique_ptr<BehaviourTree> build();
};