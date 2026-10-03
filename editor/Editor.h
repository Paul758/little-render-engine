#pragma once

#include "engine/core/ecs/Entity.h"

#include "engine/systems/EditorCameraInputSystem.h"
#include "engine/systems/EditorCameraUpdateSystem.h"

#include "editor/EditorContext.h"
#include "editor/panels/HierarchyPanel.h"
#include "editor/panels/InspectorPanel.h"

#include "editor/ComponentEditorRegistry.h"

#include "editor/component-editors/TransformEditor.h"
#include "editor/component-editors/CameraEditor.h"
#include "editor/component-editors/RenderComponentEditor.h"

#include <optional>

class World;
class HierarchySystem;
class Input;

class Editor
{
public:
    explicit Editor(World& world, HierarchySystem& hierarchySystem);
    ~Editor();

    void initialize();
    void update(Input& input, float deltaTime);

    void drawGui();

    Entity getCamera() const;
    ComponentEditorRegistry& getComponentEditorRegistry();
    
private:
    void drawEntityNode(Entity entity);

private:
    World& world_;
    HierarchySystem& hierarchySystem_;

    ComponentEditorRegistry componentEditorRegistry_;

    TransformEditor transformEditor_;
    CameraEditor cameraEditor_;
    RenderComponentEditor renderComponentEditor_;

    Entity camera_;
    EditorCameraInputSystem editorCameraInputSystem_;
    EditorCameraUpdateSystem editorCameraUpdateSystem_;

    EditorContext context_;
    HierarchyPanel hierarchyPanel_;
    InspectorPanel inspectorPanel_;
};