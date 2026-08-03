#pragma once
#include <cstddef>
#include <glad/gl.h>

class VertexBuffer
{
private:
    GLuint id = 0;

public:

    VertexBuffer(const void* data, std::size_t size);
    ~VertexBuffer();

    VertexBuffer(const VertexBuffer&) = delete;
    VertexBuffer& operator=(const VertexBuffer&) = delete;

    VertexBuffer(VertexBuffer && other) noexcept;
    VertexBuffer& operator=(VertexBuffer&& other) noexcept;
    
    void bind() const;
};