
#include "engine/assets/ModelInstantiator.h"
#include "engine/assets/NodeData.h"
#include "engine/graphics/Model.h"
#include "engine/core/ecs/World.h"
#include "engine/components/TransformComponent.h"
#include "engine/components/RenderComponent.h"
#include "engine/systems/HierarchySystem.h"

#include <vector>

ModelInstantiator::ModelInstantiator(World& world, HierarchySystem& hierarchySystem)
    : world_(world), hierarchySystem_(hierarchySystem)
{
}

Entity ModelInstantiator::instantiate(const Model& model)
{
    Entity modelRoot = world_.createEntity();
    world_.components().add<TransformComponent>(modelRoot, TransformComponent{});

    const std::vector<NodeData>& nodes = model.getNodes();

    for (std::size_t rootNodeIndex : model.getRootNodes())
    {
        const NodeData& node = nodes.at(rootNodeIndex);

        Entity nodeEntity = instantiateNode(model, rootNodeIndex);

        hierarchySystem_.setParent(nodeEntity, modelRoot);
    }

    return modelRoot;
}

Entity ModelInstantiator::instantiateNode(const Model& model, std::size_t nodeIndex)
{
    const NodeData& node = model.getNodes().at(nodeIndex);


    Entity entity = world_.createEntity();

    TransformComponent transform;
    transform.position = node.translation;
    transform.rotation = node.rotation;
    transform.scale = node.scale;

    world_.components().add<TransformComponent>(entity, transform);

    if (node.meshIndex >= 0)
    {
        const ModelMesh& mesh = model.getMesh(static_cast<std::size_t>(node.meshIndex));

        for (const ModelPrimitive& primitive : mesh.primitives)
        {
            instantiatePrimitive(model, primitive, entity);
        }
    }


    for (std::size_t childIndex : node.children)
    {
        Entity childEntity = instantiateNode(model, childIndex);

        hierarchySystem_.setParent(childEntity, entity);
    }

    return entity;
}

Entity ModelInstantiator::instantiatePrimitive(const Model& model, const ModelPrimitive& primitive, Entity nodeEntity)
{
    Entity entity = world_.createEntity();

    world_.components().add<TransformComponent>(entity, TransformComponent{});

    hierarchySystem_.setParent(entity, nodeEntity);

    RenderComponent renderComponent;
    renderComponent.mesh = primitive.mesh.get();
    renderComponent.material = &model.resolveMaterial(primitive.materialIndex);

    world_.components().add<RenderComponent>(entity, renderComponent);

    return entity;
}