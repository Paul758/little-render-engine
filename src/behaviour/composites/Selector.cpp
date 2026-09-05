#include "Selector.h"
#include "behaviour/NodeState.h"

void Selector::addChild(std::unique_ptr<Node> child)
{
    children.push_back(std::move(child));
}

NodeState Selector::update(BehaviourContext& context)
{
    for (auto& child : children)
    {
        NodeState state = child->update(context);

        if (state == NodeState::FAILURE)
        {
            continue;
        }

        if (state == NodeState::SUCCESS)
        {
            return NodeState::SUCCESS;
        }
    }

    return NodeState::FAILURE;
}