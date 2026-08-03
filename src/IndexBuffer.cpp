#include "IndexBuffer.h"

#include <utility>

IndexBuffer::IndexBuffer(const void* data, std::size_t size)
{
    glGenBuffers(1, &id);
    bind();

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
                static_cast<GLsizeiptr>(size),
                data,
                GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer()
{
    if(id != 0)
    {
        glDeleteBuffers(1, &id);
    }
}

IndexBuffer::IndexBuffer(IndexBuffer&& other) noexcept : id(std::exchange(other.id, 0))
{
}

IndexBuffer& IndexBuffer::operator=(IndexBuffer&& other) noexcept
{
    if (this != &other)
    {
        if (id != 0)
        {
            glDeleteBuffers(1, &id);
        }

        id = std::exchange(other.id, 0);
    }
    
    return *this;
}

void IndexBuffer::bind() const 
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

void IndexBuffer::unbind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}