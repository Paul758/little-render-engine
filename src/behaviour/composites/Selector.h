#pragma once

#include <vector>
#include <memory>

#include "behaviour/Node.h"

class Selector : public Node
{
public:
    void addChild(std::unique_ptr<Node> child);
    NodeState update(BehaviourContext& context) override;

private:
    std::vector<std::unique_ptr<Node>> children;
};