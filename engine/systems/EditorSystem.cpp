
#include "engine/components/camera/EditorCameraInputComponent.h"
#include "engine/systems/EditorSystem.h"

EditorSystem::EditorSystem(ComponentRegistry& registry, Entity& camera)
    : registry_(registry), camera_(camera)
{

}

void EditorSystem::update(Input& input, float deltaTime)
{
    //Check if Gizmo active

    //Check if in editor viewport
    EditorCameraInputComponent cameraInput = cameraInput = editorCameraInputSystem.build(input);
    editorCameraUpdateSystem.updateCamera(cameraInput, deltaTime);
};