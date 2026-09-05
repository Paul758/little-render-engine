#pragma once

#include <memory>

#include "NodeState.h"
#include "Node.h"

using BehaviourTreeId = std::uint32_t;

class BehaviourTree {
public:
    explicit BehaviourTree(std::unique_ptr<Node> root);
    NodeState update(BehaviourContext& context);

private:
    std::unique_ptr<Node> root;

};