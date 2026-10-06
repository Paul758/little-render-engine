#pragma once

#include <cstdint>
#include <unordered_map>

#include "engine/core/ecs/Entity.h"
#include "engine/scene/SceneData.h"

class SerializationContext
{
public:
    void registerEntity(Entity entity, SceneEntityId sceneEntityId)
    {
        entityToSceneId_[entity.index] = sceneEntityId;
    }

    SceneEntityId getSceneEntityId(Entity entity) const
    {
        return entityToSceneId_.at(entity.index);
    }

private:
    std::unordered_map<std::uint32_t, SceneEntityId> entityToSceneId_;
};