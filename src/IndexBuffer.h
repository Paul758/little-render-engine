#pragma once

#include <glad/gl.h>

#include <cstddef>

class IndexBuffer
{
private:
    GLuint id = 0;

public:
    IndexBuffer(const void* data, std::size_t size);
    ~IndexBuffer();

    IndexBuffer(const IndexBuffer&) = delete;
    IndexBuffer& operator=(const IndexBuffer&) = delete;

    IndexBuffer(IndexBuffer&& other) noexcept;
    IndexBuffer& operator=(IndexBuffer&& other) noexcept;

    void bind() const;
    static void unbind();
};