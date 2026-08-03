#pragma once
#include <string>
#include <glad/gl.h>

class Shader
{
private:
    std::string source;
    GLuint shaderID = 0; 

public:
    Shader(GLenum shaderType, const std::string& sourceFile);
    ~Shader();

    GLuint getID() const;

private:
    std::string readFile(const std::string& filePath);
    GLuint loadShader(GLenum shaderType, const std::string& source);

};