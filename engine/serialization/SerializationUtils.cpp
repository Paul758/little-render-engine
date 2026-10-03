#include "engine/serialization/SerializationUtils.h"

SerializedValue SerializationUtils::serializeVec3(const Vec3& value)
{
    SerializedValue::Object object;

    object["x"] = SerializedValue{static_cast<double>(value.x)};
    object["y"] = SerializedValue{static_cast<double>(value.y)};
    object["z"] = SerializedValue{static_cast<double>(value.z)};

    return SerializedValue{std::move(object)};
}

Vec3 SerializationUtils::deserializeVec3(const SerializedValue& value)
{
    const auto& object = value.get<SerializedValue::Object>();
    
    return Vec3{
        static_cast<float>(object.at("x").get<double>()),
        static_cast<float>(object.at("y").get<double>()),
        static_cast<float>(object.at("z").get<double>())
    };
}

SerializedValue SerializationUtils::serializeQuaternion(const Quaternion& value)
{
    SerializedValue::Object object;

    object["w"] = SerializedValue{static_cast<double>(value.w)};
    object["x"] = SerializedValue{static_cast<double>(value.x)};
    object["y"] = SerializedValue{static_cast<double>(value.y)};
    object["z"] = SerializedValue{static_cast<double>(value.z)};

    return SerializedValue{std::move(object)};
}

Quaternion SerializationUtils::deserializeQuaternion(const SerializedValue& value)
{
    const auto& object = value.get<SerializedValue::Object>();
    
    return Quaternion{
        static_cast<float>(object.at("w").get<double>()),
        static_cast<float>(object.at("x").get<double>()),
        static_cast<float>(object.at("y").get<double>()),
        static_cast<float>(object.at("z").get<double>())
    };
}
