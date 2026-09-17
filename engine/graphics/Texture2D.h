#pragma once

#include<glad/gl.h>
#include<GLFW/glfw3.h>
#include <string>

class Texture2D
{
private:
    GLuint textureID_ = 0;

public:
    explicit Texture2D(const std::string& path);
    ~Texture2D();

    Texture2D(const Texture2D&) = delete;
    Texture2D& operator=(const Texture2D&) = delete;

    Texture2D(Texture2D&& other) noexcept;
    Texture2D& operator=(Texture2D&& other) noexcept;

    void bind(GLuint unit = 0) const;
};