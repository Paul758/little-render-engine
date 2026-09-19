#pragma once

#include <glad/gl.h>

#include <vector>

#include "engine/graphics/Vertex.h"

#include "engine/graphics/MeshData.h"

class PrimitiveMesh {

public:
    static MeshData cube();
    static MeshData quad();
    static MeshData sphere(std::uint32_t slices, std::uint32_t stacks);

private:
    static std::vector<Vertex> getCubeVertices();
    static std::vector<GLuint> getCubeIndices();
    static std::vector<Vertex> getQuadVertices();
    static std::vector<GLuint> getQuadIndices();
    static std::vector<Vertex> getSphereVertices();
};