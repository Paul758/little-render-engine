#include "PrimitiveMesh.h"
#include "math/Vec3.h"

std::vector<Vertex> PrimitiveMesh::getCubeVertices()
{
    const Vec3 red     {1.0f, 0.0f, 0.0f};
    const Vec3 green   {0.0f, 1.0f, 0.0f};
    const Vec3 blue    {0.0f, 0.0f, 1.0f};
    const Vec3 yellow  {1.0f, 1.0f, 0.0f};
    const Vec3 magenta {1.0f, 0.0f, 1.0f};
    const Vec3 cyan    {0.0f, 1.0f, 1.0f};

    return {
        // Back face
        {{-0.5f, -0.5f, -0.5f}, red},
        {{ 0.5f, -0.5f, -0.5f}, red},
        {{ 0.5f,  0.5f, -0.5f}, red},
        {{-0.5f,  0.5f, -0.5f}, red},

        // Front face
        {{-0.5f, -0.5f,  0.5f}, green},
        {{ 0.5f, -0.5f,  0.5f}, green},
        {{ 0.5f,  0.5f,  0.5f}, green},
        {{-0.5f,  0.5f,  0.5f}, green},

        // Left face
        {{-0.5f, -0.5f, -0.5f}, blue},
        {{-0.5f, -0.5f,  0.5f}, blue},
        {{-0.5f,  0.5f,  0.5f}, blue},
        {{-0.5f,  0.5f, -0.5f}, blue},

        // Right face
        {{ 0.5f, -0.5f, -0.5f}, yellow},
        {{ 0.5f,  0.5f, -0.5f}, yellow},
        {{ 0.5f,  0.5f,  0.5f}, yellow},
        {{ 0.5f, -0.5f,  0.5f}, yellow},

        // Bottom face
        {{-0.5f, -0.5f, -0.5f}, magenta},
        {{ 0.5f, -0.5f, -0.5f}, magenta},
        {{ 0.5f, -0.5f,  0.5f}, magenta},
        {{-0.5f, -0.5f,  0.5f}, magenta},

        // Top face
        {{-0.5f,  0.5f, -0.5f}, cyan},
        {{-0.5f,  0.5f,  0.5f}, cyan},
        {{ 0.5f,  0.5f,  0.5f}, cyan},
        {{ 0.5f,  0.5f, -0.5f}, cyan}
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
            {1.0f, 1.0f, 1.0f},
            {0.0f, 0.0f},
            {0.0f, 0.0f}
        },
        {
            {1.0f, -1.0f, 0.0f},
            {1.0f, 1.0f, 1.0f},
            {1.0f, 0.0f},
            {0.0f, 0.0f}
        },
        {
            {1.0f, 1.0f, 0.0f},
            {1.0f, 1.0f, 1.0f},
            {1.0f, 1.0f},
            {0.0f, 0.0f}
        },
        {
            {-1.0f, 1.0f, 0.0f},
            {1.0f, 1.0f, 1.0f},
            {0.0f, 1.0f},
            {0.0f, 0.0f}
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
