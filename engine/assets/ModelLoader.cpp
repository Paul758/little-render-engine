#include "engine/assets/ModelLoader.h"
#include "engine/graphics/MeshData.h"
#include "engine/assets/ModelData.h"
#include "engine/assets/ImageData.h"

#include <stdexcept>
#include <string>

#include <tiny_gltf.h>
#include <iostream>
#include <utility>
#include <cstdint>

namespace
{
    struct AccessorData
    {
        const unsigned char* data;
        std::size_t count;
        std::size_t stride;
    };

    AccessorData getAccessorData(const tinygltf::Model& model, const tinygltf::Accessor& accessor, const std::filesystem::path& path)
    {
        const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
        const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];

        const unsigned char* data = buffer.data.data() + bufferView.byteOffset + accessor.byteOffset;

        const int byteStride = accessor.ByteStride(bufferView);

        if (byteStride < 0)
        {
            throw std::runtime_error("Invalid accessor byte stride in: " + path.string());
        }

        return AccessorData{data, accessor.count, static_cast<std::size_t>(byteStride)};
    }
}


ModelData ModelLoader::load(const std::filesystem::path& path)
{
    tinygltf::TinyGLTF loader;
    tinygltf::Model gltfModel;

    std::string error;
    std::string warning;

    bool success = false;

    if (path.extension() == ".glb")
    {
        success = loader.LoadBinaryFromFile(
            &gltfModel,
            &error,
            &warning,
            path.string()
        );
    }
    else if (path.extension() == ".gltf")
    {
        success = loader.LoadASCIIFromFile(
            &gltfModel,
            &error,
            &warning,
            path.string()
        );
    }
    else
    {
        throw std::runtime_error("Unsupported model format: " + path.string());
    }

    if (!warning.empty())
    {
        std::cout << "TinyGLTF warning: " + warning << "\n";
    }

    if (!success)
    {
        throw std::runtime_error("Failed to load model: " + path.string() + "\n" + error);
    }

    std::cout << "Loaded model: " << path << "\n";
    std::cout << "glTF meshes: " << gltfModel.meshes.size() << "\n";

    ModelData result;

    for (const tinygltf::Mesh& gltfMesh : gltfModel.meshes)
    {
        for (const tinygltf::Primitive& primitive : gltfMesh.primitives)
        {
            if (primitive.mode != TINYGLTF_MODE_TRIANGLES)
            {
                throw std::runtime_error("Only triangle primitives are currently supported in: " + path.string());
            }

            MeshData meshData;

            // POSITION

            auto positionIt = primitive.attributes.find("POSITION");

            if (positionIt == primitive.attributes.end())
            {
                throw std::runtime_error("Primitive has no POSITION attribute in: " + path.string());
            }

            const tinygltf::Accessor& positionAccessor = gltfModel.accessors[positionIt->second];

            if (positionAccessor.componentType != TINYGLTF_COMPONENT_TYPE_FLOAT ||positionAccessor.type != TINYGLTF_TYPE_VEC3)
            {
                throw std::runtime_error("Unsupported POSITION format in: " + path.string());
            }

            const AccessorData positionData = getAccessorData(gltfModel, positionAccessor, path);

            meshData.vertices.resize(positionData.count);

            for (std::size_t i = 0; i < positionData.count; ++i)
            {
                const unsigned char* vertexData = positionData.data + i * positionData.stride;

                const float* position = reinterpret_cast<const float*>(vertexData);

                meshData.vertices[i].position = Vec3{position[0], position[1], position[2]};
            }

            // NORMAL

            auto normalIt = primitive.attributes.find("NORMAL");

            if (normalIt != primitive.attributes.end())
            {
                const tinygltf::Accessor& normalAccessor = gltfModel.accessors[normalIt->second];

                if (normalAccessor.componentType != TINYGLTF_COMPONENT_TYPE_FLOAT || normalAccessor.type != TINYGLTF_TYPE_VEC3)
                {
                    throw std::runtime_error("Unsupported NORMAL format in: " + path.string());
                }

                if (normalAccessor.count != meshData.vertices.size())
                {
                    throw std::runtime_error("NORMAL count does not match POSITION count in: " + path.string());
                }

                const AccessorData normalData = getAccessorData(gltfModel, normalAccessor, path);

                for (std::size_t i = 0; i < normalData.count; ++i)
                {
                    const unsigned char* normalVertexData = normalData.data + i * normalData.stride;

                    const float* normal = reinterpret_cast<const float*>(normalVertexData);

                    meshData.vertices[i].normal = Vec3{
                        normal[0],
                        normal[1],
                        normal[2]
                    };
                }
            }

            //TEXCOORD 0
            auto texCoordIt = primitive.attributes.find("TEXCOORD_0");

            if (texCoordIt != primitive.attributes.end())
            {
                const tinygltf::Accessor& texCoordAccessor = gltfModel.accessors[texCoordIt->second];

                if (texCoordAccessor.componentType != TINYGLTF_COMPONENT_TYPE_FLOAT ||texCoordAccessor.type != TINYGLTF_TYPE_VEC2)
                {
                    throw std::runtime_error("Unsupported TEXCOORD_0 format in: " + path.string());
                }

                if (texCoordAccessor.count != meshData.vertices.size())
                {
                    throw std::runtime_error("TEXCOORD_0 count does not match POSITION count in: " + path.string());
                }

                const AccessorData texCoordData = getAccessorData(gltfModel, texCoordAccessor, path);

                for (std::size_t i = 0; i < texCoordData.count; ++i)
                {
                    const unsigned char* vertexData = texCoordData.data + i * texCoordData.stride;

                    const float* texCoord = reinterpret_cast<const float*>(vertexData);

                    meshData.vertices[i].texCoord = Vec2{texCoord[0], texCoord[1]};
                }
            }

            // Indices

            if (primitive.indices < 0)
            {
                throw std::runtime_error("Primitive has no indices in: " + path.string());
            }

            const tinygltf::Accessor& indexAccessor = gltfModel.accessors[primitive.indices];

            if (indexAccessor.type != TINYGLTF_TYPE_SCALAR)
            {
                throw std::runtime_error("Index accessor is not SCALAR in: " + path.string());
            }

            const tinygltf::BufferView& indexBufferView = gltfModel.bufferViews[indexAccessor.bufferView];
            const tinygltf::Buffer& indexBuffer = gltfModel.buffers[indexBufferView.buffer];

            const unsigned char* indexData = indexBuffer.data.data() + indexBufferView.byteOffset + indexAccessor.byteOffset;
            meshData.indices.resize(indexAccessor.count);

            switch (indexAccessor.componentType)
            {
                case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
                {
                    for (std::size_t i = 0; i < indexAccessor.count; ++i)
                    {
                        const std::uint8_t* index = reinterpret_cast<const std::uint8_t*>(indexData + i * sizeof(std::uint8_t));

                        meshData.indices[i] = static_cast<std::uint32_t>(*index);
                    }

                    break;
                }
                case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
                {
                    for (std::size_t i = 0; i < indexAccessor.count; ++i)
                    {
                        const std::uint16_t* index = reinterpret_cast<const std::uint16_t*>(indexData + i * sizeof(std::uint16_t));

                        meshData.indices[i] = static_cast<std::uint32_t>(*index);
                    }
                    
                    break;
                }
                case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
                {
                    for (std::size_t i = 0; i < indexAccessor.count; ++i)
                    {
                        const std::uint32_t* index = reinterpret_cast<const std::uint32_t*>(indexData + i * sizeof(std::uint32_t));

                        meshData.indices[i] = *index;
                    }

                    break;
                }
                default:
                {
                    throw std::runtime_error("Unsupported index component type in: " + path.string());
                }
            }
            ModelMeshData modelMeshData;
            modelMeshData.mesh = std::move(meshData);
            modelMeshData.materialIndex = primitive.material;

            result.meshes.push_back(std::move(modelMeshData)); 
        }
    }

    for (const tinygltf::Material& gltfMaterial : gltfModel.materials)
    {
        MaterialData materialData;

        const std::vector<double>& baseColor = gltfMaterial.pbrMetallicRoughness.baseColorFactor;

        if (baseColor.size() == 4)
        {
            materialData.baseColor = Vec4 {
                static_cast<float>(baseColor[0]),
                static_cast<float>(baseColor[1]),
                static_cast<float>(baseColor[2]),
                static_cast<float>(baseColor[3])
            };
        }

        const int textureIndex = gltfMaterial.pbrMetallicRoughness.baseColorTexture.index;
        
        if (textureIndex >= 0)
        {
            const tinygltf::Texture& texture = gltfModel.textures[textureIndex];
            materialData.baseColorImageIndex = texture.source;
        }

        for (const tinygltf::Image& gltfImage : gltfModel.images)
        {
            ImageData imageData;
            imageData.width = gltfImage.width;
            imageData.height = gltfImage.height;
            imageData.channels = gltfImage.component;
            imageData.pixels = gltfImage.image;

            result.images.push_back(std::move(imageData));
        }

        result.materials.push_back(std::move(materialData));
    }

    return result;
}

