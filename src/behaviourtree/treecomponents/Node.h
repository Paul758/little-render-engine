#pragma once
#include "NodeState.h"

class Node
{
public:
    virtual ~Node() = default;

    virtual NodeState update() = 0;
};