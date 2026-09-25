

#include <iostream>
#include <stdexcept>
#include <string>

#include "engine/graphics/Texture2D.h"
#include "engine/assets/ImageData.h"
#include "engine/assets/SamplerData.h"

namespace
{
    GLint toOpenGL(TextureWrap wrap)
    {
        switch (wrap)
        {
            case TextureWrap::Repeat:
                return GL_REPEAT;
            
            case TextureWrap::MirroredRepeat:
                return GL_MIRRORED_REPEAT;

            case TextureWrap::ClampToEdge:
                return GL_CLAMP_TO_EDGE;  
        }

        throw std::runtime_error("Unsupported texture wrap mode");   
    }

    GLint toOpenGL(TextureFilter filter)
    {
        switch (filter)
        {
            case TextureFilter::Nearest:
                return GL_NEAREST;

            case TextureFilter::Linear:
                return GL_LINEAR;
            
            case TextureFilter::NearestMipmapNearest:
                return GL_NEAREST_MIPMAP_NEAREST;

            case TextureFilter::LinearMipmapNearest:
                return GL_LINEAR_MIPMAP_NEAREST;
            
            case TextureFilter::NearestMipmapLinear:
                return GL_NEAREST_MIPMAP_LINEAR;

            case TextureFilter::LinearMipmapLinear:
                return GL_LINEAR_MIPMAP_LINEAR;
        }

        throw std::runtime_error("Unsupported texture filter");  
    }
}

Texture2D::Texture2D(const ImageData& image)
    : Texture2D(image, SamplerData{})
{
}

Texture2D::Texture2D(const ImageData& image, const SamplerData& sampler)
{
    glGenTextures(1, &textureID_);
    glBindTexture(GL_TEXTURE_2D, textureID_);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, image.width, image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.pixels.data());
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, toOpenGL(sampler.minFilter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, toOpenGL(sampler.magFilter));

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, toOpenGL(sampler.wrapS));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, toOpenGL(sampler.wrapT));
}

Texture2D::~Texture2D()
{
    if (textureID_ != 0)
    {
        glDeleteTextures(1, &textureID_);
    }
}

void Texture2D::bind(GLuint unit) const
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, textureID_);
}