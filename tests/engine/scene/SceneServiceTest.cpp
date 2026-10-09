#include <catch2/catch_test_macros.hpp>

#include "engine/scene/SceneService.h"

#include "engine/core/ecs/World.h"
#include "engine/core/ecs/Entity.h"
#include "engine/components/NameComponent.h"
#include "engine/components/TransformComponent.h"
#include "engine/serialization/EngineSerialization.h"
#include "engine/serialization/ComponentSerializationRegistry.h"

#include <vector>

TEST_CASE("SceneService saves and loads a scene")
{
    World world;
    Entity entity = world.createEntity();

    world.components().add<NameComponent>(entity, NameComponent{"Player"});
    TransformComponent transform;
    transform.position = Vec3{1.0f, 2.0f, 3.0f};

    world.components().add<TransformComponent>(entity, transform);

    ComponentSerializationRegistry registry;

    EngineSerialization::registerComponents(registry);

    SceneService sceneService(registry);

    sceneService.save(world, std::filesystem::temp_directory_path() / "little-renderer-scene-file-test.json");
    World emptyWorld;
    sceneService.load(emptyWorld, std::filesystem::temp_directory_path() / "little-renderer-scene-file-test.json");

    REQUIRE(emptyWorld.getAliveEntities().size() == 1);
    std::vector<Entity> loadedEntities = emptyWorld.components().getEntitiesWith<NameComponent, TransformComponent>();

    REQUIRE(loadedEntities.size() == 1);
    Entity loadedEntity = loadedEntities.at(0);

    NameComponent* nameComponent = emptyWorld.components().get<NameComponent>(loadedEntity);
    REQUIRE(nameComponent != nullptr);
    REQUIRE(nameComponent->name == "Player");

    TransformComponent* transformComponent = emptyWorld.components().get<TransformComponent>(loadedEntity);
    REQUIRE(transformComponent != nullptr);
    REQUIRE(transformComponent->position.x == 1.0f);
    REQUIRE(transformComponent->position.y == 2.0f);
    REQUIRE(transformComponent->position.z == 3.0f);
}