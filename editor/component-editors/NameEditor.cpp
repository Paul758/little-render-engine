#include "editor/component-editors/NameEditor.h"

#include <algorithm>
#include <array>

#include <imgui.h>

#include "engine/components/NameComponent.h"


void NameEditor::draw(NameComponent& name)
{
    std::array<char, 256> buffer{};

    const std::size_t copyLength = std::min(name.name.size(), buffer.size() - 1);

    std::copy_n(name.name.data(), copyLength, buffer.data());

    if (ImGui::InputText("Name", buffer.data(), buffer.size()))
    {
        name.name = buffer.data();
    }
}