#pragma once

#include <optional>

#include "Transform.h"
#include "Mesh.h"
#include "graphics/ShaderProgram.h"
#include "components/RenderComponent.h"

class GameObject
{
private:
    Transform transform_;
    std::optional<RenderComponent> renderComponent_;
public:
    GameObject() = default;

    GameObject(const GameObject&) = delete;
    GameObject& operator=(const GameObject&) = delete;

    GameObject(GameObject&&) noexcept = default;
    GameObject& operator=(GameObject&&) noexcept = default;

    Transform& getTransform();
    const Transform& getTransform() const;

    RenderComponent& addRenderComponent(const Mesh& mesh, const Material& material);
    const RenderComponent* getRenderComponent() const;
    RenderComponent* getRenderComponent();

};