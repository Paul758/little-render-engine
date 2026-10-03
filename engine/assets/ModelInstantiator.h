#pragma once

#include <cstddef>
#include "engine/core/ecs/Entity.h"

class World;
class HierarchySystem;
class Model;
struct ModelPrimitive;

class ModelInstantiator
{
public:
    ModelInstantiator(World& world, HierarchySystem& hierarchySystem);
    Entity instantiate(const Model& model);

private:
    Entity instantiateNode(const Model& modle, std::size_t nodeIndex);
    Entity instantiatePrimitive(const Model& model, const ModelPrimitive& primitive, Entity nodeEntity, size_t index);

private:
    World& world_;
    HierarchySystem& hierarchySystem_;
};