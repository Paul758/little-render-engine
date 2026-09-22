#include <catch2/catch_test_macros.hpp>

#include "engine/assets/ModelData.h"
#include "engine/assets/ModelLoader.h"
#include <iostream>

TEST_CASE("ModelLoader loads a GLB model")
{
    ModelData model = ModelLoader::load("assets/models/cube.glb");

    REQUIRE_FALSE(model.meshes.empty());
}

TEST_CASE("ModelLoader extracts vertex positions")
{
    ModelData model = ModelLoader::load("assets/models/cube.glb");

    REQUIRE_FALSE(model.meshes.empty());

    const MeshData& mesh = model.meshes[0].mesh;

    REQUIRE_FALSE(mesh.vertices.empty());
}

TEST_CASE("ModelLoader extracts vertex normals")
{
    ModelData model = ModelLoader::load("assets/models/cube.glb");

    REQUIRE_FALSE(model.meshes.empty());

    const MeshData& mesh = model.meshes[0].mesh;

    REQUIRE_FALSE(mesh.vertices.empty());

    bool hasNonZeroNormal = false;

    for (const Vertex& vertex : mesh.vertices)
    {
        const Vec3& normal = vertex.normal;

        if (normal.x != 0.0f || normal.y != 0.0f || normal.z != 0.0f)
        {
            hasNonZeroNormal = true;
            break;
        }
    }

    REQUIRE(hasNonZeroNormal);
}

TEST_CASE("ModelLoader extracts texture coordinates")
{
    ModelData model = ModelLoader::load("assets/models/cube.glb");

    REQUIRE_FALSE(model.meshes.empty());

    const MeshData& mesh = model.meshes[0].mesh;

    REQUIRE_FALSE(mesh.vertices.empty());

    bool hasNonZeroTexCoord = false;

    for (const Vertex& vertex : mesh.vertices)
    {
        const Vec2& uv = vertex.texCoord;

        if (uv.x != 0.0f || uv.y != 0.0f)
        {
            hasNonZeroTexCoord = true;
            break;
        }
    }

    REQUIRE(hasNonZeroTexCoord);
}

TEST_CASE("ModelLoader extracts indices")
{
    ModelData model = ModelLoader::load("assets/models/cube.glb");

    REQUIRE_FALSE(model.meshes.empty());

    const MeshData& mesh = model.meshes[0].mesh;

    REQUIRE_FALSE(mesh.indices.empty());
}

TEST_CASE("ModelLoader produces valid vertex indices")
{
    ModelData model = ModelLoader::load("assets/models/cube.glb");

    REQUIRE_FALSE(model.meshes.empty());

    const MeshData& mesh = model.meshes[0].mesh;

    REQUIRE_FALSE(mesh.vertices.empty());
    REQUIRE_FALSE(mesh.indices.empty());

    for (std::uint32_t index : mesh.indices)
    {
        REQUIRE(index < mesh.vertices.size());
    }
}

TEST_CASE("ModelLoader produces triangle indices")
{
    ModelData model = ModelLoader::load("assets/models/cube.glb");

    REQUIRE_FALSE(model.meshes.empty());
    const MeshData& mesh = model.meshes[0].mesh;

    REQUIRE_FALSE(mesh.indices.empty());

    REQUIRE(mesh.indices.size() % 3 == 0);
}

TEST_CASE("ModelLoader rejects unsupported model formats")
{
    REQUIRE_THROWS_AS(ModelLoader::load("assets/model/cube.obj"), std::runtime_error);
}

TEST_CASE("ModelLoader extracts materials")
{
    ModelData model = ModelLoader::load("assets/models/cube.glb");

    REQUIRE_FALSE(model.materials.empty());
}

TEST_CASE("ModelLoader produces valid material indices")
{
    ModelData model = ModelLoader::load("assets/models/cube.glb");

    for (const ModelMeshData& mesh : model.meshes)
    {
        if (mesh.materialIndex >= 0)
        {
            REQUIRE(static_cast<std::size_t>(mesh.materialIndex) < model.materials.size());
        }
    }
}

TEST_CASE("ModelLoader extracts model images")
{
    ModelData model = ModelLoader::load("assets/models/cube.glb");

    REQUIRE_FALSE(model.images.empty());

    const ImageData& image = model.images[0];

    REQUIRE(image.width > 0);
    REQUIRE(image.height > 0);
    REQUIRE_FALSE(image.pixels.empty());

}

TEST_CASE("ModelLoader produces valid base color image indices")
{
    ModelData model = ModelLoader::load("assets/models/cube.glb");

    for (const MaterialData& material : model.materials)
    {
        if (material.baseColorImageIndex >= 0)
        {
            REQUIRE(static_cast<std::size_t>(material.baseColorImageIndex) < model.images.size());
            
        }
    }
}