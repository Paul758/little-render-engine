#include "VertexArray.h"
#include <utility>

VertexArray::VertexArray()
{
    glGenVertexArrays(1, &id);
}

VertexArray::~VertexArray() 
{
    if(id != 0) 
    {
        glDeleteBuffers(1, &id);
    }
}

VertexArray::VertexArray(VertexArray&& other) noexcept : id(std::exchange(other.id, 0))
{
}

VertexArray& VertexArray::operator=(VertexArray&& other) noexcept
{
    if (this != &other)
    {
        if (id != 0)
        {
            glDeleteVertexArrays(1, &id);
        }

        id = std::exchange(other.id, 0);
    }

    return *this;
}


void VertexArray::bind() const {

    glBindVertexArray(id);
}

void VertexArray::addVertexBuffer(const VertexBuffer& vbo)
{

}


