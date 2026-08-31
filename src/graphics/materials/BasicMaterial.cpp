#include "graphics/materials/BasicMaterial.h"


BasicMaterial::BasicMaterial(ShaderProgram& shader) : shader_(&shader)
{
}

const ShaderProgram& BasicMaterial::getShader() const
{
    return *shader_;
}

void BasicMaterial::apply(const RenderContext& renderContext) const
{
}
