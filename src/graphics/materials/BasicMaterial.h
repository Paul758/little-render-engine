#pragma once

#include "graphics/materials/Material.h"
#include "graphics/Texture2D.h"

class BasicMaterial : public Material
{
private:
    ShaderProgram* shader_;

public:
    explicit BasicMaterial(ShaderProgram& shader);

    const ShaderProgram& getShader() const override;

    void apply(const RenderContext& renderContext) const override;

public:
    Vec3 albedoColor{1.0f, 1.0f, 1.0f};
    Texture2D* albedoTexture = nullptr;

    float roughness = 0.5f;
    float metalllic = 0.0f;

};