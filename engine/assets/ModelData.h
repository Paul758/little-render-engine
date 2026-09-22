#pragma once

#include <vector>

#include "engine/graphics/MeshData.h"
#include "engine/graphics/MaterialData.h"
#include "engine/graphics/Mesh.h"
#include "engine/assets/ImageData.h"

struct ModelMeshData
{
    MeshData mesh;
    int materialIndex = -1;
};

struct ModelMesh
{
    std::unique_ptr<Mesh> mesh;
    int materialIndex = -1;
};

struct ModelData
{
    std::vector<ModelMeshData> meshes;
    std::vector<MaterialData> materials;
    std::vector<ImageData> images;
};