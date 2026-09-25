#pragma once

#include <unordered_map>
#include <filesystem>
#include <memory>
#include <map>
#include <utility>

class Model;
class ShaderProgram;
class Texture2D;

class AssetManager
{
public:
    using Path = std::filesystem::path;

    explicit AssetManager(Path assetRoot);
    ~AssetManager();

    Texture2D& loadTexture(const Path& path);
    ShaderProgram& loadShader(const Path& vertexPath, const Path& fragmentPath);
    Model& loadModel(const Path& path);

private:
    Path normalizeAssetPath(const Path& path);

private:
    Path assetRoot_;
    std::unordered_map<Path, std::unique_ptr<Texture2D>> textureCache_;
    std::map<std::pair<Path, Path>, std::unique_ptr<ShaderProgram>> shaderCache_;
    std::unordered_map<Path, std::unique_ptr<Model>> modelCache_;
};

struct ShaderKey
{
    AssetManager::Path vertex;
    AssetManager::Path fragment;

    bool operator==(const ShaderKey&) const = default;
};