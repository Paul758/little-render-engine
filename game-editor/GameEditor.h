#pragma once

class ComponentEditorRegistry;

class GameEditor
{
public:
    GameEditor() = default;

    void registerComponentEditors(ComponentEditorRegistry& registry);
};
