#include "engine/serialization/EngineSerialization.h"

#include "engine/serialization/ComponentSerializationRegistry.h"
#include "engine/serialization/SerializationUtils.h"
#include "engine/components/NameComponent.h"
#include "engine/components/TransformComponent.h"

void EngineSerialization::registerComponents(ComponentSerializationRegistry& registry)
{
    registry.registerComponent<NameComponent>(
        "Name",

        [](const NameComponent& name)
        {
            SerializedValue::Object object;

            object["name"] = SerializedValue{name.name};

            return SerializedValue{std::move(object)};
        },

        [](const SerializedValue& value)
        {
            const auto& object = value.get<SerializedValue::Object>();

            return NameComponent{object.at("name").get<std::string>()};
        }
    );

    registry.registerComponent<TransformComponent>(
        "Transform",

        [](const TransformComponent& transform)
        {
            SerializedValue::Object object;

            object["Position"] = SerializationUtils::serializeVec3(transform.position);
            object["Rotation"] = SerializationUtils::serializeQuaternion(transform.rotation);
            object["Scale"] = SerializationUtils::serializeVec3(transform.scale);;

            return SerializedValue{std::move(object)};
        },

        [](const SerializedValue& value)
        {
            const auto& object = value.get<SerializedValue::Object>();

            TransformComponent transform;
            transform.position = SerializationUtils::deserializeVec3(object.at("Position"));
            transform.rotation = SerializationUtils::deserializeQuaternion(object.at("Rotation"));
            transform.scale = SerializationUtils::deserializeVec3(object.at("Scale"));
            
            return transform;
        }
    );
}