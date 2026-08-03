#include <glad/gl.h>

#include "VertexBuffer.h"
#include <utility>

VertexBuffer::VertexBuffer(const void* data, std::size_t size)
{
    glGenBuffers(1, &id);
    bind();
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(size), data, GL_STATIC_DRAW);

}

VertexBuffer::~VertexBuffer()
{
    if(id != 0)
    {
        glDeleteBuffers(1, &id);
    }
}

VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept : id(std::exchange(other.id, 0))
{
}

VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) noexcept
{
    if(this != &other)
    {
        if(id != 0)
        {
            glDeleteBuffers(1, &id);
        }

        id = std::exchange(other.id, 0);
    }

    return *this;
}

void VertexBuffer::bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, id);
}  

