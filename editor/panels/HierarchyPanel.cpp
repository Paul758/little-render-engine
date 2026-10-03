#include "editor/panels/HierarchyPanel.h"

#include <string>
#include <imgui.h>
#include "editor/EditorContext.h"
#include "engine/components/NameComponent.h"
#include "engine/core/ecs/World.h"
#include "engine/systems/HierarchySystem.h"


HierarchyPanel::HierarchyPanel(World& world, HierarchySystem& hierarchySystem)
    : world_(world), hierarchySystem_(hierarchySystem)
{
}

void HierarchyPanel::draw(EditorContext& editorContext)
{
    ImGui::Begin("Hierarchy");

    ImGui::Text("WantCaptureMouse: %s", ImGui::GetIO().WantCaptureMouse ? "true" : "false");

    for (Entity entity : world_.getAliveEntities())
    {
        if (hierarchySystem_.hasParent(entity))
        {
            continue;
        }

        drawEntityNode(entity, editorContext);
    }
    ImGui::End();
}

void HierarchyPanel::drawEntityNode(Entity entity, EditorContext& context)
{
    const auto children = hierarchySystem_.getChildren(entity);

    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;

    if (context.selectedEntity && *context.selectedEntity == entity)
    {
        flags |= ImGuiTreeNodeFlags_Selected;
    }

    if (children.empty())
    {
        flags |= ImGuiTreeNodeFlags_Leaf;
    }

    const NameComponent* nameComponent = world_.components().get<NameComponent>(entity);
    std::string label;

    if (nameComponent != nullptr)
    {
        label = nameComponent->name;
    }
    else
    {
        label = "Entity " + std::to_string(entity.index);
    }

    ImGui::PushID(static_cast<int>(entity.index));
    ImGui::PushID(static_cast<int>(entity.generation));

    const bool open = ImGui::TreeNodeEx(label.c_str(), flags);

    if (ImGui::IsItemClicked())
    {
        context.selectedEntity = entity;
    }

    if (open)
    {
        for (Entity child : hierarchySystem_.getChildren(entity))
        {
            drawEntityNode(child, context);
        }

        ImGui::TreePop();
    }

    ImGui::PopID();
    ImGui::PopID();
}