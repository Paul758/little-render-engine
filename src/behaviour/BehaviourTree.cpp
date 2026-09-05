#include "BehaviourTree.h"


BehaviourTree::BehaviourTree(std::unique_ptr<Node> rootNode)
     : root(std::move(rootNode))
{

}

NodeState BehaviourTree::update(BehaviourContext& context)
{
    return root->update(context);
}