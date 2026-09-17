#pragma once

#include "engine/behaviour/BehaviourTree.h"

constexpr BehaviourTreeId InvalidBehaviourTree = std::numeric_limits<BehaviourTreeId>::max();

struct BehaviourTreeComponent
{
    BehaviourTreeId treeId = InvalidBehaviourTree;
};