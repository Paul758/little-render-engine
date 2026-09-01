#pragma once

#include <vector>
#include <memory>

#include "Node.h"

class EvaluateAll : public Node
{
public:
    void addChild(std::unique_ptr<Node> child);
    NodeState update() override;

private:
    std::vector<std::unique_ptr<Node>> children;
};