
#include "components/camera/EditorCameraInputComponent.h"
#include "systems/EditorSystem.h"

EditorSystem::EditorSystem(ComponentRegistry& registry, Entity& camera)
    : registry_(registry), camera_(camera)
{

}

void EditorSystem::update(Input& input, float deltaTime)
{
    EditorCameraInputComponent cameraInput = cameraInput = editorCameraInputSystem.build(input);
    editorCameraUpdateSystem.updateCamera(cameraInput, deltaTime);
};