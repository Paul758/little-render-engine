#pragma once
#include "engine/behaviour/NodeState.h"
#include "engine/behaviour/BehaviourContext.h"

class Node
{
public:
    virtual ~Node() = default;

    virtual NodeState update(BehaviourContext& context) = 0;
};