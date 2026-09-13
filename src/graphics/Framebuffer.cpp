#include <stdexcept>

#include "Framebuffer.h"

Framebuffer::Framebuffer(int width, int height, TextureFilter filter) : width_(width), height_(height), filter_(filter)
{
    glGenFramebuffers(1, &framebuffer_);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);

    glGenTextures(1, &colorTexture_);
    glBindTexture(GL_TEXTURE_2D, colorTexture_);

    //Create Texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    //Define Texture Filter
    GLint glFilter = filter == TextureFilter::Nearest ? GL_NEAREST : GL_LINEAR;

    //Define parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glFilter);
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

Framebuffer::~Framebuffer()
{
    release();
}

void Framebuffer::resize(int width, int height)
{
    if (width <= 0 || height <= 0)
    {
        return;
    }

    if (width == width_ && height == height_)
    {
        return;
    }

    width_ = width;
    height_ = height;

    glBindTexture(GL_TEXTURE_2D, colorTexture_);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer_);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width_, height_);
}

void Framebuffer::bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);
}

void Framebuffer::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint Framebuffer::getColorTexture() const
{
    return colorTexture_;
}

int Framebuffer::getWidth() const
{
    return width_;
}

int Framebuffer::getHeight() const
{
    return height_;
}

GLuint Framebuffer::getId() const
{
    return framebuffer_;
}

Framebuffer::Framebuffer(Framebuffer&& other) noexcept : 
    framebuffer_(std::exchange(other.framebuffer_, 0)),
    colorTexture_(std::exchange(other.colorTexture_, 0)),
    depthBuffer_(std::exchange(other.depthBuffer_, 0)),
    width_(std::exchange(other.width_, 0)),
    height_(std::exchange(other.height_, 0))
{
}

Framebuffer& Framebuffer::operator=(Framebuffer&& other) noexcept
{
    if(this != &other)
    {
        release();

        framebuffer_ = std::exchange(other.framebuffer_, 0);
        colorTexture_ = std::exchange(other.colorTexture_, 0);
        depthBuffer_ = std::exchange(other.depthBuffer_, 0);
        width_ = std::exchange(other.width_, 0);
        height_ = std::exchange(other.height_, 0);
    }

    return *this;
}

void Framebuffer::release()
{
    if (depthBuffer_ != 0)
    {
        glDeleteRenderbuffers(1, &depthBuffer_);
        depthBuffer_ = 0;
    }

    if (colorTexture_ != 0)
    {
        glDeleteTextures(1, &colorTexture_);
        colorTexture_ = 0;
    }

    if (framebuffer_ != 0)
    {
        glDeleteFramebuffers(1, &framebuffer_);
        framebuffer_ = 0;
    }
}