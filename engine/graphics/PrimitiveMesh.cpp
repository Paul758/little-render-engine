#include "engine/graphics/PrimitiveMesh.h"
#include "engine/math/Vec3.h"
#include "engine/math/Vec2.h"
#include "engine/graphics/MeshData.h"
#include <cmath>

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

MeshData PrimitiveMesh::sphere(std::uint32_t slices, std::uint32_t stacks)
{
    MeshData data;

    constexpr float PI = 3.14159265358979323846f;

    for (std::uint32_t stack = 0; stack <= stacks; ++stack)
    {    
        float v = static_cast<float>(stack) / stacks;

        float phi = v * PI;

        for (std::uint32_t slice = 0; slice <= slices; ++slice)
        {  
            float u = static_cast<float>(slice) / slices;

            float theta = u * 2.0f * PI;

            float x = std::sin(phi) * std::cos(theta);
            
            float y = std::cos(phi);

            float z = std::sin(phi) * std::sin(theta);

            Vertex vertex;

            vertex.position = Vec3{x, y, z};
            vertex.normal = Vec3{x, y, z};
            vertex.texCoord = Vec2{u, v};

            data.vertices.push_back(vertex);

        }
    }

    std::uint32_t rowLength = slices + 1;

    for (std::uint32_t stack = 0; stack < stacks; ++stack)
    {
        for (std::uint32_t slice = 0; slice < slices; ++slice)
        {
            std::uint32_t a = stack * rowLength + slice;

            std::uint32_t b = a + 1;

            std::uint32_t c = (stack + 1) * rowLength + slice;

            std::uint32_t d = c + 1;

            // Triangle 1
            data.indices.push_back(a);
            data.indices.push_back(c);
            data.indices.push_back(b);

            // Triangle 2
            data.indices.push_back(b);
            data.indices.push_back(c);
            data.indices.push_back(d);
        }
    }

    return data;
}
