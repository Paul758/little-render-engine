#pragma once

#include "engine/scene/SceneData.h"

class World;
class ComponentSerializationRegistry;

class SceneSerializer
{
public:
    explicit SceneSerializer(const ComponentSerializationRegistry& registry);

    SceneData serialize(const World& world) const;
    void deserialize(const SceneData& scene, World& world) const;

private:
    const ComponentSerializationRegistry& registry_;
};