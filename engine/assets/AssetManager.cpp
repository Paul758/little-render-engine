#include "engine/assets/AssetManager.h"
#include <iostream>

#include "engine/assets/TextureLoader.h"

#include "engine/assets/ModelData.h"
#include "engine/assets/ModelLoader.h"
#include "engine/graphics/Model.h"
#include "engine/graphics/Texture2D.h"
#include "engine/graphics/ShaderProgram.h"
#include "engine/graphics/Mesh.h"
#include "engine/graphics/materials/BasicMaterial.h"
#include "engine/graphics/materials/Material.h"

namespace
{
    constexpr const char* StandardVertexShader = "shaders/standard.vert";
    constexpr const char* StandardFragmentShader = "shaders/standard.frag";
}

AssetManager::AssetManager(Path assetRoot)
    : assetRoot_(std::move(assetRoot).lexically_normal())
{
}

AssetManager::~AssetManager() = default;

AssetManager::Path AssetManager::normalizeAssetPath(const Path& path)
{
    if (path.is_absolute())
    {
        throw std::invalid_argument("Asset paths must be relative to the asset root");
    }
    return path.lexically_normal();
}


Texture2D& AssetManager::loadTexture(const Path& path)
{
    const Path assetPath = normalizeAssetPath(path);

    auto it = textureCache_.find(assetPath);

    if (it != textureCache_.end())
    {
        return *it->second;
    }

    Path fullPath = assetRoot_ / assetPath;

    ImageData image = TextureLoader::load(fullPath);

    auto texture = std::make_unique<Texture2D>(image);
    Texture2D& result = *texture;

    textureCache_.emplace(assetPath, std::move(texture));

    return result;
}

ShaderProgram& AssetManager::loadShader(const Path& vertexPath, const Path& fragmentPath)
{
    const Path vertexAssetPath = normalizeAssetPath(vertexPath);
    const Path fragmentAssetPath = normalizeAssetPath(fragmentPath);

    const auto key = std::make_pair(vertexAssetPath, fragmentAssetPath);

    auto it = shaderCache_.find(key);

    if (it != shaderCache_.end())
    {
        return *it->second;
    }

    const Path fullVertexPath = assetRoot_ / vertexAssetPath;
    const Path fullFragmentPath = assetRoot_ / fragmentAssetPath;

    auto shader = std::make_unique<ShaderProgram>(fullVertexPath.string(), fullFragmentPath.string());

    ShaderProgram& result = *shader;

    shaderCache_.emplace(key, std::move(shader));

    return result;
}

Model& AssetManager::loadModel(const Path& path)
{
    const Path assetPath = normalizeAssetPath(path);

    auto it = modelCache_.find(assetPath);

    if (it != modelCache_.end())
    {
        return *it->second;
    }

    ShaderProgram& shader = loadShader(StandardVertexShader, StandardFragmentShader);

    const Path fullPath = assetRoot_ / assetPath;

    ModelData data = ModelLoader::load(fullPath);

    auto model = std::make_unique<Model>();

    for (const TextureData& textureData : data.textures)
    {
        if (textureData.imageIndex < 0)
        {
            throw std::runtime_error ("Model texture has no source image");
        }
        
        const ImageData& imageData = data.images.at(static_cast<std::size_t>(textureData.imageIndex));

        SamplerData sampler;

        if (textureData.samplerIndex >= 0)
        {
            sampler = data.samplers.at(static_cast<std::size_t>(textureData.samplerIndex));
        }

        auto texture = std::make_unique<Texture2D>(imageData, sampler);

        model->addTexture(std::move(texture));
    }

    for (const MaterialData& materialData : data.materials)
    {
        auto material = std::make_unique<BasicMaterial>(shader);

        material->albedoColor = Vec3{
            materialData.baseColor.x,
            materialData.baseColor.y,
            materialData.baseColor.z
        };

        if (materialData.baseColorTextureIndex >= 0)
        {
            Texture2D& texture = model->getTexture(static_cast<std::size_t>(materialData.baseColorTextureIndex));
            material->albedoTexture = &texture;
        }

        model->addMaterial(std::move(material));
    }

    for (const ModelMeshData& meshData : data.meshes)
    {
        ModelMesh modelMesh;

        for (const ModelPrimitiveData& primitiveData : meshData.primitives)
        {
            ModelPrimitive primitive;

            primitive.mesh = std::make_unique<Mesh>(primitiveData.mesh);
            primitive.materialIndex = primitiveData.materialIndex;

            modelMesh.primitives.push_back(std::move(primitive));
        }

        model->addMesh(std::move(modelMesh));
    }

    model->setNodes(std::move(data.nodes));
    model->setRootNodes(std::move(data.rootNodes));

    Model& result = *model;

    modelCache_.emplace(assetPath, std::move(model));

    return result;
}