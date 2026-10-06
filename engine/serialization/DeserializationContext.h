#pragma once

#include <unordered_map>

#include "engine/core/ecs/Entity.h"
#include "engine/scene/SceneData.h"

class DeserializationContext
{
public:
    void registerEntity(SceneEntityId sceneEntityId, Entity entity)
    {
        sceneIdToEntity_[sceneEntityId] = entity;
    }

    Entity getEntity(SceneEntityId sceneEntityId) const
    {
        return sceneIdToEntity_.at(sceneEntityId);
    }

private:
    std::unordered_map<SceneEntityId, Entity> sceneIdToEntity_;

};