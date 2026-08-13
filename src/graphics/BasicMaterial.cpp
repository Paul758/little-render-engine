#include "graphics/BasicMaterial.h"


BasicMaterial::BasicMaterial(ShaderProgram& shader) : shader_(&shader)
{
}

const ShaderProgram& BasicMaterial::getShader() const
{
    return *shader_;
}

void BasicMaterial::apply(const Camera& camera, const GameObject& gameObject) const
{
}
