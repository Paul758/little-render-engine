#pragma once

#include <cstddef>
#include <memory>
#include <vector>

class Mesh;
struct ModelMesh;
class Material;
class Texture2D;

class Model
{
public:
    Model();
    ~Model();

    void addMesh(std::unique_ptr<Mesh> mesh, int materialIndex);
    void addMaterial(std::unique_ptr<Material> material);
    void addTexture(std::unique_ptr<Texture2D> texture);

    std::size_t getMeshCount() const;
    std::size_t getMaterialCount() const;
    std::size_t getTextureCount() const;

    Mesh& getMesh(std::size_t index);
    const Mesh& getMesh(std::size_t index) const;

    Material& getMaterial(std::size_t index);
    const Material& getMaterial(std::size_t index) const;

    Texture2D& getTexture(std::size_t index);
    const Texture2D& getTexture(std::size_t index) const;

    int getMaterialIndex(std::size_t meshIndex) const;

private:
    std::vector<ModelMesh> meshes_;
    std::vector<std::unique_ptr<Material>> materials_;
    std::vector<std::unique_ptr<Texture2D>> textures_;
};