#pragma once

#include <functional>
#include <string>
#include <vector>
#include <utility>

#include "engine/core/ecs/Entity.h"
#include "engine/core/ecs/World.h"

class ComponentEditorRegistry
{
public:
   
    template<typename T>
    void registerEditor(const std::string& name, std::function<void(T&)> drawEditor)
    {
        ComponentEditorInfo info;

        info.name = name;

        info.hasComponent = [](const World& world, Entity entity)
        {
            return world.components().has<T>(entity);
        };

        info.drawEditor = [drawEditor](World& world, Entity entity)
        {
            T* component = world.components().get<T>(entity);

            if (component == nullptr)
            {
                return;
            }

            drawEditor(*component);
        };

        componentTypes_.push_back(std::move(info));
    }

    void drawComponents(World& world, Entity entity);

private:

    struct ComponentEditorInfo
    {
        std::string name;
        std::function<bool(const World&, Entity)> hasComponent;
        std::function<void(World&, Entity)> drawEditor;
    };

    std::vector<ComponentEditorInfo> componentTypes_;
};