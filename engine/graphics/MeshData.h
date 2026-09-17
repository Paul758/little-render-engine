#pragma once

#include <glad/gl.h>
#include <vector>

#include "engine/graphics/Vertex.h"

struct MeshData 
{
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
};