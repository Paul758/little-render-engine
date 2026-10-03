#include "engine/serialization/ComponentSerializationRegistry.h"

std::vector<std::string> ComponentSerializationRegistry::getComponentNames(
    const World& world, 
    Entity entity) const
{
    std::vector<std::string> result;

    for (const ComponentSerializationInfo& info : components_)
    {
        if (info.hasComponent(world, entity))
        {
            result.push_back(info.name);
        }
    }

    return result;
}

SerializedValue ComponentSerializationRegistry::serializeComponents(const World& world, Entity entity) const
{
    SerializedValue::Object result;

    for (const ComponentSerializationInfo& info : components_)
    {
        if (!info.hasComponent(world, entity))
        {
            continue;
        }

        result[info.name] = info.serialize(world, entity);
    }

    return SerializedValue{std::move(result)};
}

void ComponentSerializationRegistry::deserializeComponents(World& world, Entity entity, const SerializedValue& value) const
{
    const auto& object = value.get<SerializedValue::Object>();

    for (const ComponentSerializationInfo& info : components_)
    {
        auto it = object.find(info.name);

        if (it == object.end())
        {
            continue;
        }

        info.deserialize(world, entity, it->second);
    }
}