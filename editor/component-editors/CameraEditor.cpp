#include "editor/component-editors/CameraEditor.h"

#include <imgui.h>
#include <algorithm>
#include "engine/components/camera/CameraComponent.h"


void CameraEditor::draw(CameraComponent& camera)
{
    constexpr float minNearPlane = 0.001f;
    constexpr float minPlaneDistance = 0.001f;
    constexpr float maxFarPlane = 10000.0f;

    ImGui::DragFloat("Field of View", &camera.fieldOfView, 0.5f, 1.0f, 179.0f);

    ImGui::DragFloat("Near Plane", &camera.nearPlane, 0.01f, minNearPlane, camera.farPlane - minPlaneDistance);
    ImGui::DragFloat("Far Plane", &camera.farPlane, 1.0f, camera.nearPlane + minPlaneDistance, maxFarPlane);

    camera.nearPlane = std::max(camera.nearPlane, minNearPlane);
    camera.farPlane = std::max(camera.farPlane, camera.nearPlane + minPlaneDistance);
}