#include <imgui.h>
#include <cstdint>

#include "editor/Editor.h"

#include "engine/core/ecs/World.h"

#include "engine/components/TransformComponent.h"
#include "engine/components/RenderComponent.h"
#include "engine/components/camera/CameraComponent.h"
#include "engine/components/camera/WorldCameraComponent.h"
#include "engine/components/NameComponent.h"

#include "engine/systems/HierarchySystem.h"

Editor::Editor(World& world, HierarchySystem& hierarchySystem)
    : world_(world),
      hierarchySystem_(hierarchySystem),
      editorCameraInputSystem_(world_.components()),
      editorCameraUpdateSystem_(world_.components()),
      hierarchyPanel_(world_, hierarchySystem_),
      inspectorPanel_(world_, componentEditorRegistry_)
{
    componentEditorRegistry_.registerEditor<TransformComponent>(
        "Transform",
        [this](TransformComponent& transform)
        {
            transformEditor_.draw(transform);
        }
    );

    componentEditorRegistry_.registerEditor<CameraComponent>(
        "Camera",
        [this](CameraComponent& camera)
        {
            cameraEditor_.draw(camera);
        }
    );

    componentEditorRegistry_.registerEditor<RenderComponent>(
        "RenderComponent",
        [this](RenderComponent& renderComponent)
        {
            renderComponentEditor_.draw(renderComponent);
        }
    );
}

Editor::~Editor() = default;

void Editor::initialize()
{
    camera_ = world_.createEntity();

    TransformComponent transformComponent;
    transformComponent.position = Vec3{0.0f, 2.0f, 5.0f};

    transformComponent.rotation = Quaternion::lookRotation(Vec3{0.0f, 0.0f, 0.0f} - transformComponent.position, Vec3{0.0f, 1.0f, 0.0f});

    world_.components().add(camera_, transformComponent);
    world_.components().add(camera_, CameraComponent{});
    world_.components().add(camera_, WorldCameraComponent{});

    world_.components().add(camera_, FreeFlyCameraComponent{});
    world_.components().add(camera_, FreeHandCameraComponent{});
    world_.components().add(camera_, EditorCameraInputComponent{});

    world_.components().add(camera_, NameComponent{"Editor camerea"});
}

void Editor::update(Input& input, float deltaTime)
{
    ImGuiIO& io = ImGui::GetIO();

    if (!io.WantCaptureMouse && !io.WantCaptureKeyboard)
    {
        editorCameraInputSystem_.build(input, camera_);
    }

    editorCameraUpdateSystem_.updateCamera(deltaTime, camera_);
}

void Editor::drawGui()
{
    hierarchyPanel_.draw(context_);
    inspectorPanel_.draw(context_);
}

Entity Editor::getCamera() const
{
    return camera_;
}

ComponentEditorRegistry& Editor::getComponentEditorRegistry()
{
    return componentEditorRegistry_;
}