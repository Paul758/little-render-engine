#pragma once

#include <vector>

#include "engine/graphics/MeshData.h"
#include "engine/assets/MaterialData.h"
#include "engine/graphics/Mesh.h"
#include "engine/assets/ImageData.h"
#include "engine/assets/TextureData.h"
#include "engine/assets/SamplerData.h"
#include "engine/assets/NodeData.h"

struct ModelPrimitiveData
{
    MeshData mesh;
    int materialIndex = -1;
};

struct ModelMeshData
{
    std::vector<ModelPrimitiveData> primitives;
};

struct ModelData
{
    std::vector<ModelMeshData> meshes;

    std::vector<MaterialData> materials;
    std::vector<ImageData> images;
    std::vector<TextureData> textures;
    std::vector<SamplerData> samplers;

    std::vector<NodeData> nodes;
    std::vector<std::size_t> rootNodes;

};