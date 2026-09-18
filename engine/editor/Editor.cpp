#include "Editor.h"

#include "engine/core/ecs/World.h"

#include "engine/components/TransformComponent.h"
#include "engine/components/camera/CameraComponent.h"
#include "engine/components/camera/WorldCameraComponent.h"

Editor::Editor(World& world)
    : world_(world),
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
}

void Editor::update(Input& input, float deltaTime)
{
    editorCameraInputSystem_.build(input, camera_);
    editorCameraUpdateSystem_.updateCamera(deltaTime, camera_);
}

Entity Editor::getCamera() const
{
    return camera_;
}
