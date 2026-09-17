#pragma once

#include <memory>

#include "engine/behaviour/NodeState.h"
#include "engine/behaviour/Node.h"

using BehaviourTreeId = std::uint32_t;

class BehaviourTree {
public:
    explicit BehaviourTree(std::unique_ptr<Node> root);
    NodeState update(BehaviourContext& context);

private:
    std::unique_ptr<Node> root;

};