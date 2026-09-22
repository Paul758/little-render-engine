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

void Model::addMesh(std::unique_ptr<Mesh> mesh, int materialIndex)
{
    ModelMesh modelMesh;
    modelMesh.mesh = std::move(mesh);
    modelMesh.materialIndex = materialIndex;

    meshes_.push_back(std::move(modelMesh));
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

Mesh& Model::getMesh(std::size_t index)
{
    return *meshes_.at(index).mesh;
}

const Mesh& Model::getMesh(std::size_t index) const
{
    return *meshes_.at(index).mesh;
}

Material& Model::getMaterial(std::size_t index)
{
    return *materials_.at(index);
}

const Material& Model::getMaterial(std::size_t index) const
{
     return *materials_.at(index);
}

Texture2D& Model::getTexture(std::size_t index)
{
    return *textures_.at(index);
}

const Texture2D& Model::getTexture(std::size_t index) const
{
    return *textures_.at(index);
}

int Model::getMaterialIndex(std::size_t index) const
{
    return meshes_.at(index).materialIndex;
}
