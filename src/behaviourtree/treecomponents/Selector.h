#pragma once

#include <vector>

#include "Node.h"
#include <memory>

class Selector : public Node
{
public:
    void addChild(std::unique_ptr<Node> child);
    NodeState update() override;

private:
    std::vector<std::unique_ptr<Node>> children;
};