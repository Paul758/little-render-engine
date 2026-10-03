#include "engine/assets/ModelLoader.h"
#include "engine/graphics/MeshData.h"
#include "engine/assets/ModelData.h"
#include "engine/assets/ImageData.h"
#include "engine/assets/NodeData.h"

#include <stdexcept>
#include <string>
#include <vector>
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

    TextureWrap getTextureWrap(int wrap, const std::filesystem::path& path)
    {
        switch (wrap)
        {
            case TINYGLTF_TEXTURE_WRAP_REPEAT:
                return TextureWrap::Repeat;
            
            case TINYGLTF_TEXTURE_WRAP_MIRRORED_REPEAT:
                return TextureWrap::MirroredRepeat;
            
            case TINYGLTF_TEXTURE_WRAP_CLAMP_TO_EDGE:
                return TextureWrap::ClampToEdge;
            
            default:
                throw std::runtime_error("Unsupported texture wrap mode in: " + path.string());
        }
    }

    TextureFilter getTextureFilter(int filter, const std::filesystem::path& path)
    {
        switch (filter)
        {
            case TINYGLTF_TEXTURE_FILTER_NEAREST:
                return TextureFilter::Nearest;
            
            case TINYGLTF_TEXTURE_FILTER_LINEAR:
                return TextureFilter::Linear;
            
            case TINYGLTF_TEXTURE_FILTER_NEAREST_MIPMAP_NEAREST:
                return TextureFilter::NearestMipmapNearest;

            case TINYGLTF_TEXTURE_FILTER_LINEAR_MIPMAP_NEAREST:
                return TextureFilter::LinearMipmapNearest;
            
            case TINYGLTF_TEXTURE_FILTER_NEAREST_MIPMAP_LINEAR:
                return TextureFilter::NearestMipmapLinear;
            
            case TINYGLTF_TEXTURE_FILTER_LINEAR_MIPMAP_LINEAR:
                return TextureFilter::LinearMipmapLinear;
            
            default:
                throw std::runtime_error("Unsupported texture filter in: " + path.string());
        }       
    }

    template<typename T>
    void validateIndex(int index, const std::vector<T>& items, const char* description, const std::filesystem::path& path)
    {
        if (index < 0 || static_cast<std::size_t>(index) >= items.size())
        {
            throw std::runtime_error(std::string("Invalid") + description + " index in: " + path.string());
        }
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
        ModelMeshData modelMeshData;

        for (const tinygltf::Primitive& primitive : gltfMesh.primitives)
        {
            if (primitive.mode != TINYGLTF_MODE_TRIANGLES)
            {
                throw std::runtime_error("Only triangle primitives are currently supported in: " + path.string());
            }

            MeshData primitiveMeshData;

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

            primitiveMeshData.vertices.resize(positionData.count);

            for (std::size_t i = 0; i < positionData.count; ++i)
            {
                const unsigned char* vertexData = positionData.data + i * positionData.stride;

                const float* position = reinterpret_cast<const float*>(vertexData);

                primitiveMeshData.vertices[i].position = Vec3{position[0], position[1], position[2]};
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

                if (normalAccessor.count != primitiveMeshData.vertices.size())
                {
                    throw std::runtime_error("NORMAL count does not match POSITION count in: " + path.string());
                }

                const AccessorData normalData = getAccessorData(gltfModel, normalAccessor, path);

                for (std::size_t i = 0; i < normalData.count; ++i)
                {
                    const unsigned char* normalVertexData = normalData.data + i * normalData.stride;

                    const float* normal = reinterpret_cast<const float*>(normalVertexData);

                    primitiveMeshData.vertices[i].normal = Vec3{
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

                if (texCoordAccessor.count != primitiveMeshData.vertices.size())
                {
                    throw std::runtime_error("TEXCOORD_0 count does not match POSITION count in: " + path.string());
                }

                const AccessorData texCoordData = getAccessorData(gltfModel, texCoordAccessor, path);

                for (std::size_t i = 0; i < texCoordData.count; ++i)
                {
                    const unsigned char* vertexData = texCoordData.data + i * texCoordData.stride;

                    const float* texCoord = reinterpret_cast<const float*>(vertexData);

                    // The 1.0f - texCoord[1] is needed to convert glTF texture-coordinate orientation to the
                    // texture-coordinate convention used by the renderer
                    primitiveMeshData.vertices[i].texCoord = Vec2{texCoord[0], 1.0f - texCoord[1]};
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
            primitiveMeshData.indices.resize(indexAccessor.count);

            switch (indexAccessor.componentType)
            {
                case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
                {
                    for (std::size_t i = 0; i < indexAccessor.count; ++i)
                    {
                        const std::uint8_t* index = reinterpret_cast<const std::uint8_t*>(indexData + i * sizeof(std::uint8_t));

                        primitiveMeshData.indices[i] = static_cast<std::uint32_t>(*index);
                    }

                    break;
                }
                case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
                {
                    for (std::size_t i = 0; i < indexAccessor.count; ++i)
                    {
                        const std::uint16_t* index = reinterpret_cast<const std::uint16_t*>(indexData + i * sizeof(std::uint16_t));

                        primitiveMeshData.indices[i] = static_cast<std::uint32_t>(*index);
                    }
                    
                    break;
                }
                case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
                {
                    for (std::size_t i = 0; i < indexAccessor.count; ++i)
                    {
                        const std::uint32_t* index = reinterpret_cast<const std::uint32_t*>(indexData + i * sizeof(std::uint32_t));

                        primitiveMeshData.indices[i] = *index;
                    }

                    break;
                }
                default:
                {
                    throw std::runtime_error("Unsupported index component type in: " + path.string());
                }
            }
            ModelPrimitiveData primitiveData;
            primitiveData.mesh = std::move(primitiveMeshData);
            primitiveData.materialIndex = primitive.material;

            if (primitive.material >= 0)
            {
                validateIndex(primitive.material, gltfModel.materials, "primitive material", path);
            }

            modelMeshData.primitives.push_back(std::move(primitiveData));
        }
        result.meshes.push_back(std::move(modelMeshData));
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
            materialData.baseColorTextureIndex  = gltfMaterial.pbrMetallicRoughness.baseColorTexture.index;

            if (materialData.baseColorTextureIndex >= 0)
            {
                validateIndex(materialData.baseColorTextureIndex, gltfModel.textures, "base color texture", path);
            }

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

        for (const tinygltf::Texture& gltfTexture : gltfModel.textures)
        {
            TextureData textureData;

            textureData.imageIndex = gltfTexture.source;
            textureData.samplerIndex = gltfTexture.sampler;

            validateIndex(textureData.imageIndex, gltfModel.images, "texture image", path);

            if (textureData.samplerIndex >= 0)
            {
                validateIndex(textureData.samplerIndex, gltfModel.samplers, "texture sampler", path);
            }

            result.textures.push_back(textureData);
        }

        result.materials.push_back(std::move(materialData));
    }

    for (const tinygltf::Sampler& gltfSampler : gltfModel.samplers)
    {
        SamplerData samplerData;

        if (gltfSampler.minFilter >= 0)
        {
            samplerData.minFilter = getTextureFilter(gltfSampler.minFilter, path);
        }

        if (gltfSampler.magFilter >= 0)
        {
            samplerData.magFilter = getTextureFilter(gltfSampler.magFilter, path);
        }

        samplerData.wrapS = getTextureWrap(gltfSampler.wrapS, path);
        samplerData.wrapT = getTextureWrap(gltfSampler.wrapT, path);

        result.samplers.push_back(samplerData);
    }

    for (const tinygltf::Node& gltfNode : gltfModel.nodes)
    {
        NodeData nodeData;

        nodeData.name = gltfNode.name;

        nodeData.meshIndex = gltfNode.mesh;

        if (nodeData.meshIndex >= 0)
        {
            validateIndex(nodeData.meshIndex, gltfModel.meshes, "node mesh", path);
        }

        if (gltfNode.translation.size() == 3)
        {
            nodeData.translation = Vec3 {
                static_cast<float>(gltfNode.translation[0]),
                static_cast<float>(gltfNode.translation[1]),
                static_cast<float>(gltfNode.translation[2])
            };
        }

        if (gltfNode.scale.size() == 3)
        {
            nodeData.scale = Vec3 {
                static_cast<float>(gltfNode.scale[0]),
                static_cast<float>(gltfNode.scale[1]),
                static_cast<float>(gltfNode.scale[2])
            };
        }

        if (gltfNode.rotation.size() == 4)
        {
            nodeData.rotation = Quaternion{
                static_cast<float>(gltfNode.rotation[3]), //w
                static_cast<float>(gltfNode.rotation[0]), //x
                static_cast<float>(gltfNode.rotation[1]), //y
                static_cast<float>(gltfNode.rotation[2])  //z
            };
        }

        for (int childIndex : gltfNode.children)
        {
            validateIndex(childIndex, gltfModel.nodes, "node child", path);

            nodeData.children.push_back(static_cast<size_t>(childIndex));
        }

        result.nodes.push_back(nodeData);
    }

    int sceneIndex = gltfModel.defaultScene;

    if (sceneIndex < 0 && !gltfModel.scenes.empty())
    {
        sceneIndex = 0;
    }

    if (sceneIndex >= 0)
    {
        validateIndex(sceneIndex, gltfModel.scenes, "scene", path);

        const tinygltf::Scene& scene = gltfModel.scenes[sceneIndex];

        for (int rootNodeIndex : scene.nodes)
        {
            validateIndex(rootNodeIndex, gltfModel.nodes, "scene root node", path);

            result.rootNodes.push_back(static_cast<std::size_t>(rootNodeIndex));
        }
    }



    return result;
}

