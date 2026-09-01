#pragma once

#include <memory>

#include "NodeState.h"
#include "Node.h"


class BehaviourTree {
public:
    explicit BehaviourTree(std::unique_ptr<Node> root);
    NodeState update();

private:
    std::unique_ptr<Node> root;

};