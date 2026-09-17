#pragma once

#include "engine/graphics/ShaderProgram.h"
#include "engine/graphics/materials/Material.h"
#include "engine/graphics/Texture2D.h"

class GrassMaterial : public Material
{
private:
    ShaderProgram* shader_;
    Texture2D* texture_;

public:
    GrassMaterial(ShaderProgram& shader, Texture2D& texture);

    const ShaderProgram& getShader() const;

    void apply(const RenderContext& renderContext) const override;
};