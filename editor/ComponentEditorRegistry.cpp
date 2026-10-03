#include "editor/ComponentEditorRegistry.h"

#include <imgui.h>

void ComponentEditorRegistry::drawComponents(World& world, Entity entity)
{
    for (ComponentEditorInfo& type : componentTypes_)
    {
        if (!type.hasComponent(world, entity))
        {
            continue;
        }

        if (ImGui::CollapsingHeader(type.name.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
        {
            type.drawEditor(world, entity);
        }
    }
}