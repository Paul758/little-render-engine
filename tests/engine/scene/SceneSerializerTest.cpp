#include <catch2/catch_test_macros.hpp>

#include "engine/core/ecs/World.h"
#include "engine/serialization/ComponentSerializationRegistry.h"
#include "engine/serialization/EngineSerialization.h"
#include "engine/scene/SceneSerializer.h"
#include "engine/components/NameComponent.h"
#include "engine/components/TransformComponent.h"
#include "engine/components/ParentComponent.h"
#include "engine/components/camera/CameraComponent.h"

TEST_CASE("SceneSerializer serializes world entities")
{
    World world;
    ComponentSerializationRegistry registry;

    EngineSerialization::registerComponents(registry);

    Entity player = world.createEntity();
    Entity camera = world.createEntity();

    world.components().add(player, NameComponent{"Player"});
    world.components().add(camera, CameraComponent{});

    SceneSerializer sceneSerializer(registry);

    const SceneData scene = sceneSerializer.serialize(world);

    REQUIRE(scene.entities.size() == 2);
    REQUIRE(scene.entities[0].id == 1);
    REQUIRE(scene.entities[1].id == 2);

    const auto& playerComponents = scene.entities[0].components.get<SerializedValue::Object>();
    REQUIRE(playerComponents.contains("Name"));
}

TEST_CASE("SceneSerializer round trips a world")
{
    ComponentSerializationRegistry registry;
    EngineSerialization::registerComponents(registry);

    SceneSerializer sceneSerializer{registry};

    World sourceWorld;
    Entity source = sourceWorld.createEntity();

    sourceWorld.components().add(source, NameComponent{"Player"});

    TransformComponent transform;
    transform.position = Vec3{1.0f, 2.0f, 3.0f};

    sourceWorld.components().add(source, transform);

    SceneData scene = sceneSerializer.serialize(sourceWorld);

    World destinationWorld;

    sceneSerializer.deserialize(scene, destinationWorld);

    REQUIRE(destinationWorld.getAliveEntities().size() == 1);

    Entity loadedEntity = destinationWorld.getAliveEntities()[0];

    const NameComponent* nameLoaded = destinationWorld.components().get<NameComponent>(loadedEntity);
    const TransformComponent* transformLoaded = destinationWorld.components().get<TransformComponent>(loadedEntity);

    REQUIRE(nameLoaded != nullptr);
    REQUIRE(transformLoaded != nullptr);

    REQUIRE(nameLoaded->name == "Player");
    REQUIRE(transformLoaded->position.x == 1.0f);
    REQUIRE(transformLoaded->position.y == 2.0f);
    REQUIRE(transformLoaded->position.z == 3.0f);
}

TEST_CASE("SceneSerializer preserves parent relationship")
{
    ComponentSerializationRegistry registry;
    EngineSerialization::registerComponents(registry);

    SceneSerializer serializer(registry);

    World sourceWorld;

    Entity parent = sourceWorld.createEntity();
    Entity child = sourceWorld.createEntity();

    sourceWorld.components().add(parent, NameComponent{"Parent"});
    sourceWorld.components().add(child, NameComponent{"Child"});
    sourceWorld.components().add(child, ParentComponent{parent});

    SceneData scene = serializer.serialize(sourceWorld);

    World destinationWorld;

    serializer.deserialize(scene, destinationWorld);

    REQUIRE(destinationWorld.getAliveEntities().size() == 2);

    std::optional<Entity> loadedParent;
    std::optional<Entity> loadedChild;

    for (Entity entity : destinationWorld.getAliveEntities())
    {
        const NameComponent* name = destinationWorld.components().get<NameComponent>(entity);

        if (name == nullptr)
            continue;

        if (name->name == "Parent")
            loadedParent = entity;

        if (name->name == "Child")
            loadedChild = entity;
    }

    REQUIRE(loadedParent.has_value());
    REQUIRE(loadedChild.has_value());

    const ParentComponent* parentComponent = destinationWorld.components().get<ParentComponent>(*loadedChild);

    REQUIRE(parentComponent != nullptr);
    REQUIRE(parentComponent->parent == *loadedParent);
}

TEST_CASE("Scene serialization preserves parent references across different runtime entities")
{
    ComponentSerializationRegistry registry;
    EngineSerialization::registerComponents(registry);

    SceneSerializer serializer(registry);

    World source;
    Entity parent = source.createEntity();
    Entity child = source.createEntity();

    source.components().add(parent, NameComponent{"Parent"});
    source.components().add(child, NameComponent{"Child"});
    source.components().add(child, ParentComponent{parent});

    const SceneData scene = serializer.serialize(source);

    World destination;
    destination.createEntity();
    destination.createEntity();

    serializer.deserialize(scene, destination);

    std::optional<Entity> loadedParent;
    std::optional<Entity> loadedChild;

    for (Entity entity : destination.getAliveEntities())
    {
        const NameComponent* name = destination.components().get<NameComponent>(entity);

        if (name == nullptr)
            continue;

        if (name->name == "Parent")
            loadedParent = entity;

        if (name->name == "Child")
            loadedChild = entity;       
    }

    REQUIRE(loadedParent.has_value());
    REQUIRE(loadedChild.has_value());

    const ParentComponent* parentComponent = destination.components().get<ParentComponent>(*loadedChild);
    REQUIRE(parentComponent != nullptr);
    REQUIRE(parentComponent->parent == *loadedParent);

}