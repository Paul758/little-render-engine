#include "editor/component-editors/RenderComponentEditor.h"
#include "engine/components/RenderComponent.h"
#include <imgui.h>

void RenderComponentEditor::draw(const RenderComponent& renderComponent)
{
    if (renderComponent.mesh != nullptr)
    {
        ImGui::Text("Mesh: assigned");
    }
    else
    {
        ImGui::TextDisabled("Mesh: none");
    }

    if (renderComponent.material != nullptr)
    {
        ImGui::Text("Material: assigned");
    }
    else
    {
        ImGui::TextDisabled("Material: none");
    }
}