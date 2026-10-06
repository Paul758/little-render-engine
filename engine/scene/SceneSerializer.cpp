#include "engine/scene/SceneSerializer.h"
#include "engine/core/ecs/Entity.h"
#include "engine/core/ecs/World.h"
#include "engine/serialization/SerializedValue.h"
#include "engine/serialization/ComponentSerializationRegistry.h"
#include "engine/serialization/SerializationContext.h"
#include "engine/serialization/DeserializationContext.h"
#include <vector>
#include <unordered_map>
#include <cstdint>

SceneSerializer::SceneSerializer(const ComponentSerializationRegistry& registry)
    : registry_(registry)
{
}

SceneData SceneSerializer::serialize(const World& world) const
{
    SceneData scene;

    SerializationContext context;

    SceneEntityId nextId = 1;

    for (Entity entity : world.getAliveEntities())
    {
        context.registerEntity(entity, nextId++);
    }

    for (Entity entity : world.getAliveEntities())
    {
        SceneEntityData entityData;

        entityData.id = context.getSceneEntityId(entity);
        entityData.components = registry_.serializeComponents(world, entity, context);

        scene.entities.push_back(std::move(entityData));
    }

    return scene;
}

void SceneSerializer::deserialize(const SceneData& scene, World& world) const
{
    DeserializationContext context;

    //Pass 1
    for (const SceneEntityData& entityData : scene.entities)
    {
        Entity entity = world.createEntity();

        context.registerEntity(entityData.id, entity);

    }

    //Pass 2
    for (const SceneEntityData& entityData : scene.entities)
    {
        Entity entity = context.getEntity(entityData.id);

        registry_.deserializeComponents(world, entity, entityData.components, context);
    }
}