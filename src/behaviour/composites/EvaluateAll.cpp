#include "EvaluateAll.h"

void EvaluateAll::addChild(std::unique_ptr<Node> child)
{
    children.push_back(std::move(child));
}

NodeState EvaluateAll::update(BehaviourContext& context)
{
    for (auto& child : children)
    {
        NodeState state = child->update(context);
    }

    return NodeState::SUCCESS;
}