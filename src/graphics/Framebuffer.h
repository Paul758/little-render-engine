#pragma once

#include <glad/gl.h>
#include "graphics/TextureFilter.h"

class Framebuffer
{
public:
    Framebuffer(int width, int height, TextureFilter filter = TextureFilter::Linear);
    ~Framebuffer();

    Framebuffer(const Framebuffer&) = delete;
    Framebuffer& operator=(const Framebuffer&) = delete;

    Framebuffer(Framebuffer&& other) noexcept;
    Framebuffer& operator=(Framebuffer&& other) noexcept;

    void bind() const;
    static void unbind();

    GLuint getColorTexture() const;
    GLuint getId() const;

    int getWidth() const;
    int getHeight() const;

    void resize(int width, int height);

    void release();

private:
    GLuint framebuffer_ = 0;
    GLuint colorTexture_ = 0;
    GLuint depthBuffer_ = 0;

    int width_ = 0;
    int height_ = 0;

    TextureFilter filter_;
};