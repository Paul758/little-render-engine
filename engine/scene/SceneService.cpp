#include "engine/scene/SceneService.h"

#include <filesystem>

#include "engine/scene/SceneData.h"
#include "engine/scene/SceneJson.h"
#include "engine/scene/SceneFile.h"
#include "engine/scene/SceneSerializer.h"

SceneService::SceneService(const ComponentSerializationRegistry& registry)
    : registry_(registry)
{
}

void SceneService::save(
        const World& world,
        const std::filesystem::path& path) const
{
    SceneSerializer sceneSerializer(registry_);
    std::string sceneJson = SceneJson::serialize(sceneSerializer.serialize(world));
    SceneFile::write(path, sceneJson);
}

void SceneService::load(
        World& world,
        const std::filesystem::path& path) const
{
    SceneData sceneData = SceneJson::deserialize(SceneFile::read(path));
    SceneSerializer sceneSerializer(registry_);
    sceneSerializer.deserialize(sceneData, world);
}