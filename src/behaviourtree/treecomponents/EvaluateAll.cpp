

#include "behaviourtree/treecomponents/EvaluateAll.h"

void EvaluateAll::addChild(std::unique_ptr<Node> child)
{
    children.push_back(std::move(child));
}

NodeState EvaluateAll::update()
{
    for (auto& child : children)
    {
        NodeState state = child->update();
    }

    return NodeState::SUCCESS;
}