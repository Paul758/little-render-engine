#pragma once

#include <string>
#include <glad/gl.h>

#include "math/Mat4.h"

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

    GLuint getID() const;

};