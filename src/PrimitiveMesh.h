#pragma once

#include <glad/gl.h>

#include <vector>

#include "Vertex.h"

#include "MeshData.h"

class PrimitiveMesh {

public:
    static MeshData cube();
    static MeshData quad();

private:
    static std::vector<Vertex> getCubeVertices();
    static std::vector<GLuint> getCubeIndices();
    static std::vector<Vertex> getQuadVertices();
    static std::vector<GLuint> getQuadIndices();
};