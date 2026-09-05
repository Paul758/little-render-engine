#pragma once

#include <vector>
#include <memory>

#include "BehaviourTree.h"

class BehaviourTreeRegistry
{
public:
    BehaviourTreeId add(std::unique_ptr<BehaviourTree> tree)
    {
        BehaviourTreeId id = static_cast<BehaviourTreeId>(trees.size());

        trees.push_back(std::move(tree));

        return id;
    }

    BehaviourTree* get(BehaviourTreeId id)
    {
        if (id >= trees.size())
        {
            return nullptr;
        }

        return trees[id].get();
    }

private:
    std::vector<std::unique_ptr<BehaviourTree>> trees;
};