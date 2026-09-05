#pragma once

#include<vector>

#include "behaviour/NodeState.h"
#include "behaviour/Node.h"

struct TestComponent
{
    int value = 0;
};

class RecordingNode : public Node
{
public:
    NodeState update(BehaviourContext& context) override
    {
        evaluatedEntities.push_back(context.entity);
        return NodeState::SUCCESS;
    }

    std::vector<Entity> evaluatedEntities;
};

class ModifyTestComponentNode : public Node
{
public:
    NodeState update(BehaviourContext& context) override
    {
        auto* component = context.get<TestComponent>();

        if (component == nullptr)
        {
            return NodeState::FAILURE;
        }

        component->value = 42;

        return NodeState::SUCCESS;
    }
};

class AlwaysFailNode : public Node
{
    NodeState update(BehaviourContext& context) override
    {
        return NodeState::FAILURE;
    }
};

class AlwaysSucceedNode : public Node
{
    NodeState update(BehaviourContext& context) override
    {
        return NodeState::SUCCESS;
    }
};