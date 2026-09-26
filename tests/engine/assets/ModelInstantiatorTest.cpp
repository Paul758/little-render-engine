#include <catch2/catch_test_macros.hpp>
#include <algorithm>

#include "engine/assets/ModelInstantiator.h"
#include "engine/assets/ModelData.h"
#include "engine/assets/NodeData.h"
#include "engine/core/ecs/World.h"
#include "engine/components/TransformComponent.h"
#include "engine/graphics/Model.h"
#include "engine/systems/HierarchySystem.h"


TEST_CASE("ModelInstantiator instantiates root nodes")
{
    World world;
    HierarchySystem hierarchySystem(world.components());

    Model model;

    NodeData node;

    node.translation = Vec3{1.0f, 2.0f, 3.0f};

    model.setNodes({node});
    model.setRootNodes({0});

    ModelInstantiator instantiator(world, hierarchySystem);

    Entity modelRoot = instantiator.instantiate(model);

    REQUIRE(world.isAlive(modelRoot));

    const TransformComponent* rootTransform = world.components().get<TransformComponent>(modelRoot);

    REQUIRE(rootTransform != nullptr);

    std::vector<Entity> children = hierarchySystem.getChildren(modelRoot);

    REQUIRE(children.size() == 1);

    Entity nodeEntity = children[0];

    const TransformComponent* nodeTransform = world.components().get<TransformComponent>(nodeEntity);

    REQUIRE(nodeTransform != nullptr);

    REQUIRE(nodeTransform->position.x == 1.0f);
    REQUIRE(nodeTransform->position.y == 2.0f);
    REQUIRE(nodeTransform->position.z == 3.0f);
}

TEST_CASE("ModeInstantiator instantiates a model with a hierarchy")
{
    World world;
    HierarchySystem hierarchySystem(world.components());
    ModelInstantiator instantiator(world, hierarchySystem);

    Model model;

    NodeData rootNode;

    rootNode.translation = Vec3{10.0f, 0.0f, 0.0f};
    rootNode.children = {1};

    NodeData childNode;

    childNode.translation = Vec3{2.0f, 0.0f, 0.0f};
    childNode.children = {2};

    NodeData grandChildNode;

    grandChildNode.translation = Vec3{3.0f, 0.0f, 0.0f};

    model.setNodes({rootNode, childNode, grandChildNode});

    model.setRootNodes({0});

    Entity modelRoot = instantiator.instantiate(model);
    std::vector<Entity> modelRootChildren = hierarchySystem.getChildren(modelRoot);
    REQUIRE(modelRootChildren.size() == 1);
    
    Entity rootEntity = modelRootChildren[0];
    std::vector<Entity> rootChildren = hierarchySystem.getChildren(rootEntity);
    REQUIRE(rootChildren.size() == 1);

    Entity childEntity = rootChildren[0];
    std::vector<Entity> childChildren = hierarchySystem.getChildren(childEntity);
    REQUIRE(childChildren.size() == 1);

    Entity grandChildEntity = childChildren[0];

    TransformComponent* rootTransform = world.components().get<TransformComponent>(rootEntity);
    TransformComponent* childTransform = world.components().get<TransformComponent>(childEntity);
    TransformComponent* grandChildTransform = world.components().get<TransformComponent>(grandChildEntity);

    REQUIRE(rootTransform != nullptr);
    REQUIRE(childTransform != nullptr);
    REQUIRE(grandChildTransform != nullptr);

    REQUIRE(rootTransform->position.x == 10.0f);
    REQUIRE(childTransform->position.x == 2.0f);
    REQUIRE(grandChildTransform->position.x == 3.0f);
}

TEST_CASE("ModelInstantiator creates primitive entity for model mesh")
{
    World world;
    HierarchySystem  hierarchySystem(world.components());

    Model model;

    NodeData node;
    node.meshIndex = 0;

    model.setNodes({node});
    model.setRootNodes({0});

    ModelMesh mesh;
    ModelPrimitive primitive;
    mesh.primitives.push_back(std::move(primitive));

    model.addMesh(std::move(mesh));

    ModelInstantiator instantiator(world, hierarchySystem);

    Entity modelRoot = instantiator.instantiate(model);

    // Model root -> glTF Node
    std::vector<Entity> rootChildren = hierarchySystem.getChildren(modelRoot);

    REQUIRE(rootChildren.size() == 1);

    Entity nodeEntity = rootChildren[0];

    // glTF node -> primitive entity

    std::vector<Entity> nodeChildren = hierarchySystem.getChildren(nodeEntity);
    REQUIRE(nodeChildren.size() == 1);

    Entity primitiveEntity = nodeChildren[0];

    REQUIRE(world.components().has<TransformComponent>(primitiveEntity));
}

TEST_CASE("ModelInstantiator handles node referencing empty mesh")
{
    World world;
    HierarchySystem  hierarchySystem(world.components());

    Model model;

    NodeData node;
    node.meshIndex = 0;

    model.setNodes({node});
    model.setRootNodes({0});

    ModelMesh mesh;
    model.addMesh(std::move(mesh));

    ModelInstantiator instantiator(world, hierarchySystem);

    Entity modelRoot = instantiator.instantiate(model);

    // Model root -> glTF Node
    std::vector<Entity> rootChildren = hierarchySystem.getChildren(modelRoot);

    REQUIRE(rootChildren.size() == 1);

    Entity nodeEntity = rootChildren[0];

    std::vector<Entity> primitiveEntities = hierarchySystem.getChildren(nodeEntity);

    REQUIRE(primitiveEntities.empty());
}