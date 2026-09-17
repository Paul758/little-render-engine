#include <iostream>
#include <utility>
#include "engine/graphics/Mesh.h"
#include "engine/graphics/VertexBuffer.h"
#include "engine/graphics/VertexArray.h"


Mesh::Mesh(const MeshData& data) :
    vbo(data.vertices.data() , data.vertices.size() * sizeof(Vertex)),    
    vao(),
    vertexCount(static_cast<GLsizei>(data.vertices.size())),
    indexCount(0)
    
{
    vao.bind(); 
    vbo.bind();
    
    if(!data.indices.empty())
    {
        ebo.emplace(data.indices.data(), data.indices.size() * sizeof(GLuint));
        indexCount = static_cast<GLsizei>(data.indices.size());
    }

    configureVertexLayout();
    
}

void Mesh::configureVertexLayout()
{
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, position)));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, normal)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, texCoord)));
    glEnableVertexAttribArray(2);

}

void Mesh::draw() const 
{
    vao.bind(); 

    if(ebo.has_value())
    {
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
    }
    else
    {
        glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    } 
}

Mesh::~Mesh() = default;