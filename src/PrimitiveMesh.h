#pragma once

#include <glad/gl.h>

#include <vector>

#include "Vertex.h"

class PrimitiveMesh {

public:
    static std::vector<Vertex> getCubeVertices();
    static std::vector<GLuint> getCubeIndices();
    static std::vector<Vertex> getQuadVertices();
    static std::vector<GLuint> getQuadIndices();
};