#include "systems/BehaviourTreeSystem.h"
#include "components/BehaviourTreeComponent.h"

BehaviourTreeSystem::BehaviourTreeSystem(ComponentRegistry& components, BehaviourTreeRegistry& trees)
    : components_(components), trees_(trees)
{

}

void BehaviourTreeSystem::update()
{
    auto entities = components_.getEntitiesWith<BehaviourTreeComponent>();

    for (Entity entity : entities)
    {
        BehaviourTreeComponent* behaviourTreeComponent = components_.get<BehaviourTreeComponent>(entity);

        if (behaviourTreeComponent == nullptr)
        {
            continue;
        }

        BehaviourContext context {entity, components_};

        BehaviourTree* tree = trees_.get(behaviourTreeComponent->treeId);

        if (tree == nullptr)
        {
            continue;
        }

        tree->update(context);
    }
}