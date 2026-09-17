#pragma once
#include <glad/gl.h>

#include <cstddef>
#include <vector>
#include <optional>

#include "engine/graphics/Vertex.h"
#include "engine/graphics/VertexBuffer.h"
#include "engine/graphics/VertexArray.h"
#include "engine/graphics/MeshData.h"
#include "engine/graphics/IndexBuffer.h"

class Mesh 
{
private:
    VertexBuffer vbo;
    VertexArray vao;
    std::optional<IndexBuffer> ebo;

    GLsizei vertexCount = 0;
    GLsizei indexCount = 0;

    void configureVertexLayout();

public:
    explicit Mesh(const MeshData& vertices);
    ~Mesh();

    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;
    Mesh(Mesh&& other) noexcept = default;
    Mesh& operator=(Mesh&& other) noexcept = default;

    void draw() const;



};