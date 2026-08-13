#pragma once

#include <string>
#include <glad/gl.h>

#include "math/Mat4.h"
#include "math/Vec3.h"
#include "math/Vec2.h"

class ShaderProgram
{
private:
    GLuint programID;

public:

    ShaderProgram(const std::string& vertexShaderPath, const std::string& fragmentShaderPath); 
    
    ~ShaderProgram(); 
    
    ShaderProgram(const ShaderProgram&) = delete;
    ShaderProgram& operator=(const ShaderProgram&) = delete;

    ShaderProgram(ShaderProgram&& other) noexcept;
    ShaderProgram& operator=(ShaderProgram&& other) noexcept;

    void use() const;
    
    void setMat4(const std::string& name, const Mat4& matrix) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setVec3(const std::string& name, const Vec3& vector) const;
    void setVec2(const std::string& name, const Vec2& vector) const;

    GLuint getID() const;

};