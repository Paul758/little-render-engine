#pragma once
#include "graphics/Material.h"

class BasicMaterial : public Material
{
private:
    ShaderProgram* shader_;

public:
    explicit BasicMaterial(ShaderProgram& shader);

    const ShaderProgram& getShader() const override;

    void apply(const Camera& camera, const GameObject& gameObject) const override;
};