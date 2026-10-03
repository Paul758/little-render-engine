#include "editor/panels/InspectorPanel.h"

#include "editor/EditorContext.h"
#include "editor/component-editors/NameEditor.h"
#include "editor/ComponentEditorRegistry.h"

#include "engine/core/ecs/World.h"
#include "engine/components/NameComponent.h"


#include <imgui.h>


InspectorPanel::InspectorPanel(World& world, ComponentEditorRegistry& componentTypeRegistry)
   : world_(world), componentEditorRegistry_(componentTypeRegistry)
{
}
    
void InspectorPanel::draw(EditorContext& context)
{
    ImGui::Begin("Inspector");

    if (!context.selectedEntity)
    {
        ImGui::Text("No entity selected");
        ImGui::End();
        return;
    }

    const Entity entity = *context.selectedEntity;

    if (!world_.isAlive(entity))
    {
        context.selectedEntity.reset();
        ImGui::Text("No entity selected");
        ImGui::End();
        return;
    }

    NameComponent* name = world_.components().get<NameComponent>(entity);

    if (name != nullptr)
    {
        nameEditor_.draw(*name);
    }
    else
    {
        ImGui::TextDisabled("Name: <unnamed>");
    }

    componentEditorRegistry_.drawComponents(world_, entity);

    ImGui::End();
}