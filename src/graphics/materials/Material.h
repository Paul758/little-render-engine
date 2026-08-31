#pragma once

#include "graphics/RenderContext.h"

class Camera;
class ShaderProgram;
class GameObject;

class Material
{
public:
    virtual ~Material() = default;

    virtual const ShaderProgram& getShader() const = 0;

    virtual void apply(const RenderContext& renderContext) const = 0;

};