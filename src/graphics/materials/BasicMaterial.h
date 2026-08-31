#pragma once
#include "graphics/materials/Material.h"

class BasicMaterial : public Material
{
private:
    ShaderProgram* shader_;

public:
    explicit BasicMaterial(ShaderProgram& shader);

    const ShaderProgram& getShader() const override;

    void apply(const RenderContext& renderContext) const override;
};