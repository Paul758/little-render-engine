#pragma once

#include <string>
#include "engine/scene/SceneData.h"
#include "engine/serialization/SerializedValue.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class SceneJson
{
public:
    static std::string serialize(const SceneData& scene);
    static SceneData deserialize(const std::string& json);

private:
    static json toJson(const SerializedValue& value);
    static SerializedValue fromJson(const json& jsonString);
};