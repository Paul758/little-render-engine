#pragma once

#include <glad/gl.h>

class PixelFramebuffer
{
public:
    PixelFramebuffer(int width, int height);
    ~PixelFramebuffer();

    PixelFramebuffer(const PixelFramebuffer&) = delete;
    PixelFramebuffer& operator=(const PixelFramebuffer&) = delete;

    PixelFramebuffer(PixelFramebuffer&& other) noexcept;
    PixelFramebuffer& operator=(PixelFramebuffer&& other) noexcept;

    void bind() const;
    static void unbind();

    GLuint getColorTexture() const;

    int getWidth() const;
    int getHeight() const;

    GLuint getId() const;

private:
    GLuint framebuffer_ = 0;
    GLuint colorTexture_ = 0;
    GLuint depthBuffer_ = 0;

    int width_ = 0;
    int height_ = 0;
};