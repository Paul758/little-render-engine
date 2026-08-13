#include "graphics/ShaderProgram.h"
#include "Camera.h"
#include "graphics/GrassMaterial.h"
#include "graphics/Texture2D.h"


ShaderProgram* shader_;
Texture2D* texture_;


GrassMaterial::GrassMaterial(ShaderProgram& shader, Texture2D& texture) : shader_(&shader), texture_(&texture)
{
}

const ShaderProgram& GrassMaterial::getShader() const
{
    return *shader_;
}

void GrassMaterial::apply(const Camera& camera, const GameObject& gameObject) const
{
    texture_->bind(0);

    shader_->setInt("grassTexture", 0);
    shader_->setVec3("cameraRight", camera.getRight());
    shader_->setVec3("cameraUp", camera.getUp());
    shader_->setFloat("spriteWidth", 16.0f * PixelSettings::worldUnitsPerPixel());
    shader_->setFloat("spriteHeight", 16.0f * PixelSettings::worldUnitsPerPixel());
}
