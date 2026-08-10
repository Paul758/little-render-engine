#pragma once

#include <cstddef>
#include <vector>

#include "TerrainCell.h"
#include "MeshData.h"



class Terrain
{
public:
    Terrain(std::size_t width, std::size_t height, float cellSize);

    TerrainCell& getCell(std::size_t x, std::size_t z);
    const TerrainCell& getCell(std::size_t x, std::size_t z) const;

    void generateTerrain();
    MeshData createMeshData() const;
    Vec3 getGrassColor(GrassType type) const;
    float samplePatchValue(std::size_t x, std::size_t z) const;
    float randomValue(int x, int z) const;
    float sampleNoise(float x, float z) const;
    bool hasDifferentNeighbor(std::size_t x, std::size_t z) const;
    MeshData createGrassMeshData() const;

private:
    
    std::size_t width_;
    std::size_t height_;
    float cellSize_;

    std::vector<TerrainCell> cells_;

    void addGrassQuad(MeshData& data, const Vec3& center, const Vec3& color) const;

    static float lerp(float a, float b, float t);
    static float smoothStep(float t);
};