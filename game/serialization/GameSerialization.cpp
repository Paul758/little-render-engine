#include "game/serialization/GameSerialization.h"

#include "engine/serialization/ComponentSerializationRegistry.h"
#include "game/components/HealthComponent.h"

void GameSerialization::registerComponents(ComponentSerializationRegistry& registry)
{
    registry.registerComponent<HealthComponent>(
        "Health",
        [](const HealthComponent& health)
        {
            SerializedValue::Object object;

            object["currentHealth"] = SerializedValue{static_cast<double>(health.currentHealth)};
            object["maxHealth"] = SerializedValue{static_cast<double>(health.maxHealth)};

            return SerializedValue{std::move(object)};
        },
        [](const SerializedValue& value)
        {
            const auto& object = value.get<SerializedValue::Object>();
            HealthComponent health;

            health.currentHealth = static_cast<float>(object.at("currentHealth").get<double>());
            health.maxHealth = static_cast<float>(object.at("maxHealth").get<double>());

            return health;
        }
    );
}