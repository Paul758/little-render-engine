#include "PrimitiveMesh.h"
#include "math/Vec3.h"
#include "MeshData.h"

MeshData PrimitiveMesh::cube()
{
    return MeshData{
        .vertices = getCubeVertices(),
        .indices = getCubeIndices()
    };
}

MeshData PrimitiveMesh::quad()
{
    return MeshData{
        .vertices = getQuadVertices(),
        .indices = getQuadIndices()
    };
}

std::vector<Vertex> PrimitiveMesh::getCubeVertices()
{
    return {
        // Back face, normal = -Z
        {{-0.5f, -0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {0.0f, 0.0f}},
        {{ 0.5f, -0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {1.0f, 0.0f}},
        {{ 0.5f,  0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {1.0f, 1.0f}},
        {{-0.5f,  0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {0.0f, 1.0f}},

        // Front face, normal = +Z
        {{-0.5f, -0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {0.0f, 0.0f}},
        {{ 0.5f, -0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {1.0f, 0.0f}},
        {{ 0.5f,  0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {1.0f, 1.0f}},
        {{-0.5f,  0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {0.0f, 1.0f}},

        // Left face, normal = -X
        {{-0.5f, -0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}, {0.0f, 0.0f}},
        {{-0.5f, -0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 0.0f}},
        {{-0.5f,  0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 1.0f}},
        {{-0.5f,  0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}, {0.0f, 1.0f}},

        // Right face, normal = +X
        {{ 0.5f, -0.5f, -0.5f}, { 1.0f,  0.0f,  0.0f}, {0.0f, 0.0f}},
        {{ 0.5f,  0.5f, -0.5f}, { 1.0f,  0.0f,  0.0f}, {0.0f, 1.0f}},
        {{ 0.5f,  0.5f,  0.5f}, { 1.0f,  0.0f,  0.0f}, {1.0f, 1.0f}},
        {{ 0.5f, -0.5f,  0.5f}, { 1.0f,  0.0f,  0.0f}, {1.0f, 0.0f}},

        // Bottom face, normal = -Y
        {{-0.5f, -0.5f, -0.5f}, { 0.0f, -1.0f,  0.0f}, {0.0f, 0.0f}},
        {{ 0.5f, -0.5f, -0.5f}, { 0.0f, -1.0f,  0.0f}, {1.0f, 0.0f}},
        {{ 0.5f, -0.5f,  0.5f}, { 0.0f, -1.0f,  0.0f}, {1.0f, 1.0f}},
        {{-0.5f, -0.5f,  0.5f}, { 0.0f, -1.0f,  0.0f}, {0.0f, 1.0f}},

        // Top face, normal = +Y
        {{-0.5f,  0.5f, -0.5f}, { 0.0f,  1.0f,  0.0f}, {0.0f, 0.0f}},
        {{-0.5f,  0.5f,  0.5f}, { 0.0f,  1.0f,  0.0f}, {0.0f, 1.0f}},
        {{ 0.5f,  0.5f,  0.5f}, { 0.0f,  1.0f,  0.0f}, {1.0f, 1.0f}},
        {{ 0.5f,  0.5f, -0.5f}, { 0.0f,  1.0f,  0.0f}, {1.0f, 0.0f}}
    };
}

std::vector<GLuint> PrimitiveMesh::getCubeIndices()
{
    return {
        0,  2,  1,   0,  3,  2,  // Back
        4,  5,  6,   4,  6,  7,  // Front
        8,  9, 10,   8, 10, 11,  // Left
        12, 13, 14,  12, 14, 15,  // Right
        16, 17, 18,  16, 18, 19,  // Bottom
        20, 21, 22,  20, 22, 23   // Top
    };
}

std::vector<Vertex> PrimitiveMesh::getQuadVertices()
{
    return {
        {
            {-1.0f, -1.0f, 0.0f},
            { 0.0f,  0.0f, 1.0f},
            { 0.0f,  0.0f}
        },
        {
            { 1.0f, -1.0f, 0.0f},
            { 0.0f,  0.0f, 1.0f},
            { 1.0f,  0.0f}
        },
        {
            { 1.0f,  1.0f, 0.0f},
            { 0.0f,  0.0f, 1.0f},
            { 1.0f,  1.0f}
        },
        {
            {-1.0f,  1.0f, 0.0f},
            { 0.0f,  0.0f, 1.0f},
            { 0.0f,  1.0f}
        }
    };
}

std::vector<GLuint> PrimitiveMesh::getQuadIndices()
{
    return {
        0, 1, 2,
        0, 2, 3
    };
}
