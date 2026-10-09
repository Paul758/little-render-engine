#pragma once

#include "engine/core/ecs/World.h"
#include "engine/scene/SceneSerializer.h"
#include <filesystem>

class SceneService
{
public:

    SceneService(const ComponentSerializationRegistry& registry);

    void save(
        const World& world,
        const std::filesystem::path& path) const;
    
    void load(
        World& world,
        const std::filesystem::path& path) const;

private:
    const ComponentSerializationRegistry& registry_;
};