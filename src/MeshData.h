#pragma once

#include <glad/gl.h>
#include <vector>

#include "Vertex.h"

struct MeshData 
{
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
};