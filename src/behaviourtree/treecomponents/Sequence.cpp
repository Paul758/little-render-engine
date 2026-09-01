#include "Sequence.h"
#include "NodeState.h"

void Sequence::addChild(std::unique_ptr<Node> child)
{
    children.push_back(std::move(child));
}

NodeState Sequence::update()
{
    for (auto& child : children)
    {
        NodeState state = child->update();

        if (state == NodeState::FAILURE)
        {
            return NodeState::FAILURE;
        }
    }

    return NodeState::SUCCESS;
}