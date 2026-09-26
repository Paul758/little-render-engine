
#include <utility>
#include <stdexcept>

#include "engine/graphics/Model.h"
#include "engine/graphics/Mesh.h"
#include "engine/assets/ModelData.h"
#include "engine/graphics/materials/Material.h"
#include "engine/graphics/Texture2D.h"

Model::Model() = default;

Model::~Model() = default;

void Model::addMaterial(std::unique_ptr<Material> material)
{
    materials_.push_back(std::move(material));
}

void Model::addMesh(ModelMesh mesh)
{
    meshes_.push_back(std::move(mesh));
}

void Model::addTexture(std::unique_ptr<Texture2D> texture)
{
    textures_.push_back(std::move(texture));
}

std::size_t Model::getMeshCount() const
{
    return meshes_.size();
}

std::size_t Model::getMaterialCount() const
{
    return materials_.size();
}

ModelMesh& Model::getMesh(std::size_t index)
{
    return meshes_.at(index);
}

const ModelMesh& Model::getMesh(std::size_t index) const
{
    return meshes_.at(index);
}

Material& Model::getMaterial(std::size_t index)
{
    return *materials_.at(index);
}

const Material& Model::getMaterial(std::size_t index) const
{
     return *materials_.at(index);
}

Material& Model::resolveMaterial(int materialIndex)
{
    if (materialIndex == -1)
    {
        return *defaultMaterial_;
    }

    if (materialIndex < -1)
    {
        throw std::out_of_range("Invalid material index");
    }

    return getMaterial(static_cast<std::size_t>(materialIndex));
}

const Material& Model::resolveMaterial(int materialIndex) const
{
    if (materialIndex == -1)
    {
        return *defaultMaterial_;
    }

    if (materialIndex < -1)
    {
        throw std::out_of_range("Invalid material index");
    }

    return getMaterial(static_cast<std::size_t>(materialIndex));
}

Texture2D& Model::getTexture(std::size_t index)
{
    return *textures_.at(index);
}

const Texture2D& Model::getTexture(std::size_t index) const
{
    return *textures_.at(index);
}

const std::vector<NodeData>& Model::getNodes() const
{
    return nodes_;
}

const std::vector<std::size_t>& Model::getRootNodes() const
{
    return rootNodes_;
}

void Model::setNodes(std::vector<NodeData> nodes)
{
    nodes_ = std::move(nodes);
}

void Model::setRootNodes(std::vector<std::size_t> rootNodes)
{
    rootNodes_ = std::move(rootNodes);
}

void Model::setDefaultMaterial(std::unique_ptr<Material> material)
{
    defaultMaterial_ = std::move(material);
}
