#pragma once
#include "NodeState.h"
#include "BehaviourContext.h"

class Node
{
public:
    virtual ~Node() = default;

    virtual NodeState update(BehaviourContext& context) = 0;
};