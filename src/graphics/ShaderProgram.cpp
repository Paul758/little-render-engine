#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "graphics/Shader.h"
#include "graphics/ShaderProgram.h"
#include "math/Mat4.h"
#include <math/Vec2.h>

ShaderProgram::ShaderProgram(const std::string& vertexShaderPath, const std::string& fragmentShaderPath) 
{
    Shader vertexShader(GL_VERTEX_SHADER, vertexShaderPath);
    Shader fragmentShader(GL_FRAGMENT_SHADER, fragmentShaderPath);

    programID = glCreateProgram();
    glAttachShader(programID, vertexShader.getID());
    glAttachShader(programID, fragmentShader.getID());

    glLinkProgram(programID);

    GLint success;
    glGetProgramiv(programID, GL_LINK_STATUS, &success);

    if(!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(programID, sizeof(infoLog), nullptr, infoLog);
        std::cerr << infoLog << std::endl;

        glDeleteProgram(programID);
        programID = 0;
    }
}

ShaderProgram::~ShaderProgram() 
{
    if(programID != 0) 
    {
        glDeleteProgram(programID);
    }
}
    
ShaderProgram::ShaderProgram(ShaderProgram&& other) noexcept : programID (std::exchange(other.programID, 0))
{
}

ShaderProgram& ShaderProgram::operator=(ShaderProgram&& other) noexcept
{
    if (this != &other)
    {
        if (programID != 0)
        {
            glDeleteProgram(programID);
        }

        programID = std::exchange(other.programID, 0);
    }

    return *this;
}

void ShaderProgram::use() const 
{
    glUseProgram(programID);
}

void ShaderProgram::setMat4(const std::string& name, const Mat4& matrix) const
{
    const GLint location = glGetUniformLocation(programID, name.c_str());

    if(location == -1)
    {
        std::cerr << "Uniform not found: " << name << '\n';
        return;
    }

    glUniformMatrix4fv(location, 1, GL_FALSE, matrix.data());
}

void ShaderProgram::setInt(const std::string& name, int value) const
{
    const GLint location = glGetUniformLocation(programID, name.c_str());

    if(location == -1)
    {
        std::cerr << "Uniform not found: " << name << '\n';
        return;
    }

    glUniform1i(location, value);
}

void ShaderProgram::setFloat(const std::string& name, float value) const
{
    const GLint location = glGetUniformLocation(programID, name.c_str());

    if(location == -1)
    {
        std::cerr << "Uniform not found: " << name << '\n';
        return;
    }

    glUniform1f(location, value);
}


void ShaderProgram::setVec3(const std::string& name, const Vec3& vector) const
{
    const GLint location = glGetUniformLocation(programID, name.c_str());

    if(location == -1)
    {
        std::cerr << "Uniform not found: " << name << '\n';
        return;
    }

    glUniform3f(location, vector.x, vector.y, vector.z);
}

void ShaderProgram::setVec2(const std::string& name, const Vec2& vector) const
{
    const GLint location = glGetUniformLocation(programID, name.c_str());

    if(location == -1)
    {
        std::cerr << "Uniform not found: " << name << '\n';
        return;
    }

    glUniform2f(location, vector.x, vector.y);
}

GLuint ShaderProgram::getID() const {
    return programID;
}

