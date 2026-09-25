#pragma once

#include <cstddef>
#include <memory>
#include <vector>

class Mesh;
struct ModelMesh;
class Material;
class Texture2D;
struct NodeData;

struct ModelPrimitive
{
    std::unique_ptr<Mesh> mesh;
    int materialIndex = -1;
};

struct ModelMesh
{
    std::vector<ModelPrimitive> primitives;
};

class Model
{
public:
    Model();
    ~Model();

    void addMesh(ModelMesh mesh);
    void addMaterial(std::unique_ptr<Material> material);
    void addTexture(std::unique_ptr<Texture2D> texture);

    std::size_t getMeshCount() const;
    std::size_t getMaterialCount() const;
    std::size_t getTextureCount() const;

    ModelMesh& getMesh(std::size_t index);
    const ModelMesh& getMesh(std::size_t index) const;

    Material& getMaterial(std::size_t index);
    const Material& getMaterial(std::size_t index) const;

    Texture2D& getTexture(std::size_t index);
    const Texture2D& getTexture(std::size_t index) const;

    const std::vector<NodeData>& getNodes() const;
    const std::vector<std::size_t>& getRootNodes() const;

    void setNodes(std::vector<NodeData> nodes);
    void setRootNodes(std::vector<std::size_t> rootNodes);

private:
    std::vector<ModelMesh> meshes_;
    std::vector<std::unique_ptr<Material>> materials_;
    std::vector<std::unique_ptr<Texture2D>> textures_;

    std::vector<NodeData> nodes_;
    std::vector<std::size_t> rootNodes_;
};