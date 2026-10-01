#include <imgui.h>
#include <cstdint>

#include "app/editor/Editor.h"

#include "engine/core/ecs/World.h"

#include "engine/components/TransformComponent.h"
#include "engine/components/RenderComponent.h"
#include "engine/components/camera/CameraComponent.h"
#include "engine/components/camera/WorldCameraComponent.h"
#include "engine/components/ParentComponent.h"
#include "engine/components/NameComponent.h"

#include "engine/systems/HierarchySystem.h"

Editor::Editor(World& world, HierarchySystem& hierarchySystem)
    : world_(world),
      hierarchySystem_(hierarchySystem),
      editorCameraInputSystem_(world_.components()),
      editorCameraUpdateSystem_(world_.components())
{
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
    
    ImGui::Begin("Hierarchy");

    ImGui::Text("WantCaptureMouse: %s", ImGui::GetIO().WantCaptureMouse ? "true" : "false");

    for (Entity entity : world_.getAliveEntities())
    {
        if (hierarchySystem_.hasParent(entity))
        {
            continue;
        }

        drawEntityNode(entity);
    }
    ImGui::End();
}

Entity Editor::getCamera() const
{
    return camera_;
}

void Editor::drawEntityNode(Entity entity)
{
    const auto children = hierarchySystem_.getChildren(entity);
    const bool hasChildren = !children.empty();

    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

    if (!hasChildren)
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

    const bool hasRender = world_.components().has<RenderComponent>(entity);
    const bool hasCamera = world_.components().has<CameraComponent>(entity);

    const bool open = ImGui::TreeNodeEx(reinterpret_cast<void*>(static_cast<std::uintptr_t>(entity.index)),
       flags,
       "%s",
       label.c_str()
        );

    if (open)
    {
        for (Entity child : hierarchySystem_.getChildren(entity))
        {
            drawEntityNode(child);
        }

        ImGui::TreePop();
    }
}