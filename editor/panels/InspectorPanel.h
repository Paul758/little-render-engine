#pragma once

#include "editor/EditorContext.h"
#include "editor/component-editors/NameEditor.h"
class World;
class ComponentEditorRegistry;

class InspectorPanel
{
public:
    InspectorPanel(World& world, ComponentEditorRegistry& componentTypeRegistry);
    void draw(EditorContext& context);
private:
    World& world_;

    ComponentEditorRegistry& componentEditorRegistry_;
    NameEditor nameEditor_;
};