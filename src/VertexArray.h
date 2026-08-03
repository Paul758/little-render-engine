#pragma once
#include <glad/gl.h>

#include "VertexBuffer.h"

class VertexArray
{
private:
    GLuint id = 0;

public:
    VertexArray();
    ~VertexArray();

    VertexArray(const VertexArray&) = delete;
    VertexArray& operator=(const VertexArray&) = delete;

    VertexArray(VertexArray&& other) noexcept;
    VertexArray& operator=(VertexArray&& other) noexcept;

    void addVertexBuffer(const VertexBuffer& vbo);
    void bind() const;
};