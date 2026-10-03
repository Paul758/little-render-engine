#pragma once

#include <utility>
#include <string>
#include <functional>
#include <vector>

#include "engine/core/ecs/Entity.h"
#include "engine/core/ecs/World.h"
#include "engine/serialization/SerializedValue.h"

class ComponentSerializationRegistry
{
public:

    template <typename T>
    void registerComponent(const std::string& name,
         std::function<SerializedValue(const T&)> serialize,
         std::function<T(const SerializedValue&)> deserialize)
    {
        ComponentSerializationInfo info;

        info.name = name;

        info.hasComponent = [](const World& world, Entity entity)
        {
            return world.components().has<T>(entity);
        };

        info.serialize = [serialize](const World& world, Entity entity)
        {
            const T* component = world.components().get<T>(entity);

            if (component == nullptr)
            {
                return SerializedValue{};
            }

            return serialize(*component);
        };

        info.deserialize = [deserialize](World& world, Entity entity, const SerializedValue& value)
        {
            T component = deserialize(value);

            world.components().add<T>(entity, component);
        };

        components_.push_back(std::move(info));
    }

    std::vector<std::string> getComponentNames(const World& world, Entity entity) const;

    SerializedValue serializeComponents(const World& world, Entity entity) const;
    void deserializeComponents(World& world, Entity entity, const SerializedValue& value) const;

private:
    struct ComponentSerializationInfo
    {
        std::string name;

        std::function<bool(const World&, Entity)> hasComponent;
        std::function<SerializedValue(const World&, Entity)> serialize;
        std::function<void(World&, Entity, const SerializedValue&)> deserialize;
    };

    std::vector<ComponentSerializationInfo> components_;
};

