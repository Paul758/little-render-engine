#include "graphics/materials/TerrainMaterial.h"


TerrainMaterial::TerrainMaterial(ShaderProgram& shader) : shader_(&shader)
{
}

const ShaderProgram& TerrainMaterial::getShader() const 
{
    return *shader_;
}

void TerrainMaterial::apply(const RenderContext& renderContext) const 
{

}
