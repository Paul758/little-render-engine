#include "editor/component-editors/TransformEditor.h"

#include <imgui.h>

#include "engine/components/TransformComponent.h"
#include "engine/math/Quaternion.h"
#include "engine/math/Vec3.h"

void TransformEditor::draw(TransformComponent& transform)
{
    ImGui::DragFloat3("Position", &transform.position.x, 0.1f);
    
    Vec3 eulerDegrees = transform.rotation.toEuler();

    if (ImGui::DragFloat3("Rotation", &eulerDegrees.x, 0.5f))
    {
        transform.rotation = Quaternion::fromEuler(eulerDegrees);
    }

    ImGui::DragFloat3("Scale", &transform.scale.x, 0.1f);
}