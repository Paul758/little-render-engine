#include "GameObject.h"

Transform& GameObject::getTransform()
{
    return transform_;
}

const Transform& GameObject::getTransform() const
{
    return transform_;
}

RenderComponent& GameObject::addRenderComponent(const Mesh& mesh, const ShaderProgram& shader)
{
   return renderComponent_.emplace(RenderComponent{&mesh, &shader});
}

const RenderComponent* GameObject::getRenderComponent() const
{
    return renderComponent_? &*renderComponent_ : nullptr;
}

RenderComponent* GameObject::getRenderComponent()
{
    return renderComponent_? &*renderComponent_ : nullptr;
}