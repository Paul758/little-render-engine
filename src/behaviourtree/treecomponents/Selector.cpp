#include "Selector.h"
#include "NodeState.h"

void Selector::addChild(std::unique_ptr<Node> child)
{
    children.push_back(std::move(child));
}

NodeState Selector::update()
{
    for (auto& child : children)
    {
        NodeState state = child->update();

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