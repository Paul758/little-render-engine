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

        entityJson["components"] = serializeComponents(entity.components);

        root["entities"].push_back(std::move(entityJson));
    }

    return root.dump(4);
}

json SceneJson::serializeComponents(const SerializedValue& value)
{
    json result;

    if(value.is<SerializedValue::Object>())
    {
       const SerializedValue::Object& object = value.get<SerializedValue::Object>();

       for (const auto& [key, childValue] : object)
       {
            result[key] = serializeComponents(childValue);
       }

       return result;
    }
    else if(value.is<SerializedValue::Array>())
    {
        json array = json::array();
        for(const SerializedValue& arrayValue : value.get<SerializedValue::Array>())
        {
            array.push_back(serializeComponents(arrayValue));
        }
        return array;
    }
    else
    {
        if(value.is<std::nullptr_t>())
            return json(nullptr);
        else if(value.is<bool>())
            return json(value.get<bool>());
        else if(value.is<double>())
            return json(value.get<double>());
        else if(value.is<std::uint64_t>())
            return json(value.get<std::uint64_t>());
        else if(value.is<std::string>())
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
        scene.entities.push_back(std::move(entity));
    }

    return scene;
}