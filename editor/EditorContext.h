#pragma once

#include <optional>
#include "engine/core/ecs/Entity.h"

struct EditorContext
{
    std::optional<Entity> selectedEntity;
};
