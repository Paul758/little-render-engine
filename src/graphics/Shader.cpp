#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <glad/gl.h>

#include "Shader.h"

Shader::Shader(GLenum shaderType, const std::string& sourceFile) {
    const std::string source = readFile(sourceFile);
    shaderID = loadShader(shaderType, source);
}

GLuint Shader::getID() const 
{
    return shaderID;
}

Shader::~Shader()
{
    if(shaderID != 0) 
    {
        glDeleteShader(shaderID);
    }
}

std::string Shader::readFile(const std::string& filePath)
{
    std::cout << "Opening file: " << filePath << '\n';
    std::ifstream file(filePath);

    if(!file.is_open()) {
        throw std::runtime_error("Could not open file " + filePath);
    }

    std::stringstream stream;
    stream << file.rdbuf();

    return stream.str();
}

GLuint Shader::loadShader(GLenum shaderType, const std::string& source) {
    
    const char* sourceCStr = source.c_str();

    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &sourceCStr, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if(!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, sizeof(infoLog), nullptr, infoLog);
        std::cerr << "Shader compilation failed:\n" << infoLog << std::endl;

        glDeleteShader(shader);

        throw std::runtime_error(
        std::string("Shader compilation failed:\n") + infoLog
        );
    }

    return shader;
}

