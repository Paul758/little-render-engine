#pragma once

class Camera;
class ShaderProgram;
class GameObject;

class Material
{
public:
    virtual ~Material() = default;

    virtual const ShaderProgram& getShader() const = 0;

    virtual void apply(const Camera& camera, const GameObject& object) const = 0;

};