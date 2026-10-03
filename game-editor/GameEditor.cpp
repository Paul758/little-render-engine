#include "game-editor/GameEditor.h"
#include <imgui.h>
#include <algorithm>

#include "editor/ComponentEditorRegistry.h"
#include "game/components/HealthComponent.h"


void GameEditor::registerComponentEditors(ComponentEditorRegistry& registry)
{
    registry.registerEditor<HealthComponent>(
        "Health",
        [](HealthComponent& health)
        {
            ImGui::DragFloat(
                "Current",
                &health.currentHealth,
                1.0f,
                0.0f,
                health.maxHealth
            );

            ImGui::DragFloat(
                "Maximum",
                &health.maxHealth,
                1.0f,
                0.0f
            );
            health.maxHealth = std::max(0.0f, health.maxHealth);
            health.currentHealth = std::clamp(health.currentHealth, 0.0f, health.maxHealth);
        } 
    );
}