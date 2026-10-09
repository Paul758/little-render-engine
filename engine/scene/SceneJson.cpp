#include "engine/scene/SceneJson.h"

#include <nlohmann/json.hpp>
#include <string>
#include <stdexcept>

using json = nlohmann::json;

std::string SceneJson::serialize(const SceneData& scene)
{
    json root;
    root["entities"] = json::array();

    for (const SceneEntityData& entity : scene.entities)
    {
        json entityJson;
        entityJson["id"] = entity.id;

        entityJson["components"] = toJson(entity.components);

        root["entities"].push_back(std::move(entityJson));
    }

    return root.dump(4);
}

json SceneJson::toJson(const SerializedValue& value)
{

    if (value.is<SerializedValue::Object>())
    {
       json object = json::object();

       for (const auto& [key, childValue] : value.get<SerializedValue::Object>())
       {
            object[key] = toJson(childValue);
       }

       return object;
    }
    else if (value.is<SerializedValue::Array>())
    {
        json array = json::array();
        for(const SerializedValue& arrayValue : value.get<SerializedValue::Array>())
        {
            array.push_back(toJson(arrayValue));
        }
        return array;
    }
    else
    {
        if (value.is<std::nullptr_t>())
            return json(nullptr);
        else if (value.is<bool>())
            return json(value.get<bool>());
        else if (value.is<double>())
            return json(value.get<double>());
        else if (value.is<std::uint64_t>())
            return json(value.get<std::uint64_t>());
        else if (value.is<std::int64_t>())
            return json(value.get<std::int64_t>());
        else if (value.is<std::string>())
            return json(value.get<std::string>());

        throw std::runtime_error("Unsupported SerializedValue type");
    }          
}

SceneData SceneJson::deserialize(const std::string& jsonString)
{
    const json root = json::parse(jsonString);

    SceneData scene;

    for (const json& entityJson : root.at("entities"))
    {
        SceneEntityData entity;

        entity.id = entityJson.at("id").get<SceneEntityId>();

        entity.components = fromJson(entityJson.at("components"));

        scene.entities.push_back(std::move(entity));
    }

    return scene;
}

SerializedValue SceneJson::fromJson(const json& jsonString)
{
    if (jsonString.is_object())
    {
        SerializedValue::Object result;
        for (auto& [key, value] : jsonString.items())
        {
            result[key] = fromJson(value);
        }
        return result;
    }
    else if (jsonString.is_array())
    {
        SerializedValue::Array result;
        for (auto& value : jsonString.items())
            result.push_back(fromJson(value.value()));
        return result;
    }
    else
    {
        if (jsonString.is_null())
            return SerializedValue{};
        else if (jsonString.is_boolean())
            return SerializedValue{jsonString.get<bool>()};
        else if (jsonString.is_number())
            if (jsonString.is_number_float())
                return SerializedValue{jsonString.get<double>()};
            else if (jsonString.is_number_unsigned())
                return SerializedValue{jsonString.get<uint64_t>()};
            else
                return SerializedValue{jsonString.get<int64_t>()};
        else if (jsonString.is_string())
            return SerializedValue{jsonString.get<std::string>()};
    }
    throw std::runtime_error("Unsupported type in JSON string: " + jsonString.dump(4));
}
