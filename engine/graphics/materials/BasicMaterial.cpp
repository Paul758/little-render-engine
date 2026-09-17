#include "engine/graphics/materials/BasicMaterial.h"
#include "engine/graphics/ShaderProgram.h"
#include "engine/components/lighting/DirectionalLightData.h"

BasicMaterial::BasicMaterial(ShaderProgram& shader) : shader_(&shader)
{
}

const ShaderProgram& BasicMaterial::getShader() const
{
    return *shader_;
}

void BasicMaterial::apply(const RenderContext& renderContext) const
{
    
    shader_->setVec3("material.albedoColor", albedoColor);

    const bool hasTexture = albedoTexture != nullptr;

    shader_->setBool("material.hasAlbedoTexture", hasTexture);

    if (hasTexture)
    {
        albedoTexture->bind(0);
        shader_->setInt("material.albedoTexture", 0);
    }

    const LightingData& lighting = renderContext.lighting;

    if(lighting.directionalLights.empty())
    {
        return;
    }

    const DirectionalLightData& light = lighting.directionalLights[0];

    shader_->setVec3("directionalLight.direction", light.direction);
    shader_->setVec3("directionalLight.color", light.color);
    shader_->setFloat("directionalLight.intensity", light.intensity);

    shader_->setVec3("ambientColor", lighting.ambientColor);
}
