#include <stdexcept>

#include "PixelFrameBuffer.h"

PixelFramebuffer::PixelFramebuffer(int width, int height) : width_(width), height_(height)
{
    glGenFramebuffers(1, &framebuffer_);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);

    glGenTextures(1, &colorTexture_);
    glBindTexture(GL_TEXTURE_2D, colorTexture_);

    //Create Texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    //Define parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture_, 0);

    glGenRenderbuffers(1, &depthBuffer_);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer_);

    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width_, height_);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthBuffer_);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        throw std::runtime_error ("Pixel framebuffer is incomplete");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

PixelFramebuffer::~PixelFramebuffer()
{
    if (depthBuffer_ != 0)
    {
        glDeleteRenderbuffers(1, &depthBuffer_);
    }

    if (colorTexture_ != 0)
    {
        glDeleteTextures(1, &colorTexture_);
    }

    if (framebuffer_ != 0)
    {
        glDeleteFramebuffers(1, &framebuffer_);
    }
}

void PixelFramebuffer::bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);
    glViewport(0, 0, width_, height_);
}

void PixelFramebuffer::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint PixelFramebuffer::getColorTexture() const
{
    return colorTexture_;
}

int PixelFramebuffer::getWidth() const
{
    return width_;
}

int PixelFramebuffer::getHeight() const
{
    return height_;
}

GLuint PixelFramebuffer::getId() const
{
    return framebuffer_;
}

PixelFramebuffer::PixelFramebuffer(PixelFramebuffer&& other) noexcept : framebuffer_(std::exchange(other.framebuffer_, 0))
{
}

PixelFramebuffer& PixelFramebuffer::operator=(PixelFramebuffer&& other) noexcept
{
    if(this != &other)
    {
        if(framebuffer_ != 0)
        {
            glDeleteBuffers(1, &framebuffer_);
        }

        framebuffer_ = std::exchange(other.framebuffer_, 0);
    }

    return *this;
}