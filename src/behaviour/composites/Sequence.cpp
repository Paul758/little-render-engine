#include "Sequence.h"
#include "behaviour/NodeState.h"

void Sequence::addChild(std::unique_ptr<Node> child)
{
    children.push_back(std::move(child));
}

NodeState Sequence::update(BehaviourContext& context)
{
    for (auto& child : children)
    {
        NodeState state = child->update(context);

        if (state == NodeState::FAILURE)
        {
            return NodeState::FAILURE;
        }
    }

    return NodeState::SUCCESS;
}