#include <cmath>
#include <climits>

#include "Terrain.h"


Terrain::Terrain(std::size_t width, std::size_t height, float cellSize) :
    width_(width), height_(height), cellSize_(cellSize), cells_(width * height)
{
}

TerrainCell& Terrain::getCell(std::size_t x, std::size_t z)
{
    return cells_[z * width_ + x];
}

const TerrainCell& Terrain::getCell(std::size_t x, std::size_t z) const
{
    return cells_[z * width_ + x];
}

void Terrain::generateTerrain() 
{
      for (std::size_t z = 0; z < height_; ++z)
    {
        for (std::size_t x = 0; x < width_; ++x)
        {
            const float fx = static_cast<float>(x);
            const float fz = static_cast<float>(z);

            const float dark = sampleGrassField(fx, fz, 0.0f, 0.0f);
            const float medium = sampleGrassField(fx, fz, 37.0f, 71.0f) + 0.08f;
            const float light = sampleGrassField(fx, fz, 113.0f, 191.0f);

            if (dark > medium && dark > light)
            {
                getCell(x, z).grassType = GrassType::Dark;
            }
            else if (medium > light)
            {
                getCell(x, z).grassType = GrassType::Medium;
            }
            else
            {
                getCell(x, z).grassType = GrassType::Light;
            }
        }
    }
}

Vec3 Terrain::getGrassColor(GrassType type) const
{
    switch (type)
    {
        case GrassType::Dark:
            //return {0.24f, 0.36f, 0.12f};
            return {0.18f, 0.32f, 0.12f};
        case GrassType::Medium:
            return {0.28f, 0.45f, 0.18f};
        case GrassType::Light:
            return {0.42f, 0.58f, 0.24f};
    }
    return {1.0f, 0.0f, 1.0f};
}

MeshData Terrain::createMeshData() const
{
    MeshData data;

    const float terrainWidth = static_cast<float>(width_) * cellSize_;
    const float terrainDepth = static_cast<float>(height_) * cellSize_;
    const float startX = -terrainWidth * 0.5f;
    const float startZ = -terrainDepth * 0.5f;

    for (std::size_t z = 0; z < height_; ++z)
    {
        for (std::size_t x = 0; x < width_; ++x)
        {
            const TerrainCell& cell = getCell(x, z);

            //const Vec3 color = getGrassColor(cell.grassType);
            const Vec3 unusedColor{1.0f, 1.0f, 1.0f};
            
            const float worldX = startX + static_cast<float>(x) * cellSize_;
            const float worldZ = startZ + static_cast<float>(z) * cellSize_;

            const Vec3 p0 {worldX, 0.0f, worldZ};
            const Vec3 p1 {worldX + cellSize_, 0.0f, worldZ};
            const Vec3 p3 {worldX + cellSize_, 0.0f, worldZ + cellSize_};
            const Vec3 p2 {worldX, 0.0f, worldZ + cellSize_};

            const GLuint baseIndex = static_cast<GLuint> (data.vertices.size());

            data.vertices.push_back({p0, unusedColor});
            data.vertices.push_back({p1, unusedColor});
            data.vertices.push_back({p2, unusedColor});
            data.vertices.push_back({p3, unusedColor});

            data.indices.push_back({baseIndex + 0});
            data.indices.push_back({baseIndex + 1});
            data.indices.push_back({baseIndex + 2});

            data.indices.push_back({baseIndex + 1});
            data.indices.push_back({baseIndex + 2});
            data.indices.push_back({baseIndex + 3});
            
        }
    }
    return data;
}

GrassType Terrain::getGrassTypeAt(float worldX, float worldZ) const
{
    const Vec2 position{worldX, worldZ};

    const float dark = sampleGrassField(position.x, position.y, 0.0f, 0.0f);

    const float medium = sampleGrassField(position.x, position.y, 37.0f, 71.0f);// + 0.08;

    const float light = sampleGrassField(position.x, position.y, 113.0f, 191.0f);

    if (dark > medium && dark > light) 
    {
        return GrassType::Dark;
    }
    else if (medium > light)
    {
        return GrassType::Medium;
    }
    else
    {
        return GrassType::Light;
    }
}

float Terrain::samplePatchValue(std::size_t x, std::size_t z) const
{
    //0.08, 0.15,
    constexpr float frequency = 0.08f;

    const float value = sampleNoise(static_cast<float>(x) * frequency, static_cast<float>(z) * frequency);
    return value;
}

float Terrain::randomValue(int x, int z) const
{
    unsigned int hash = static_cast<unsigned int>(x) * 374761393u + static_cast<unsigned int>(z) * 668265263u;

    hash = (hash ^ (hash >> 13u)) * 1274126177u;
    hash ^= hash >> 16u;

    return static_cast<float>(hash) / static_cast<float>(UINT_MAX);
}

float Terrain::sampleGrassField(float x, float z, float offsetX, float offsetZ) const
{
    float value = 0.0f;

    value += sampleNoise(x * 0.33f + offsetX, z * 0.33f + offsetZ) * 0.8f;

    value += sampleNoise(x * 0.5f + offsetX, z * 0.5f + offsetZ) * 0.2f;


    return value;
}

float Terrain::lerp(float a, float b, float t)
{
    return a + (b - a) * t;
}

float Terrain::smoothStep(float t)
{
    return t * t * (3.0f - 2.0f * t);
}

float Terrain::sampleNoise(float x, float z) const
{
    const int x0 = static_cast<int>(std::floor(x));
    const int z0 = static_cast<int>(std::floor(z));

    const int x1 = x0 + 1;
    const int z1 = z0 + 1;

    const float v00 = randomValue(x0, z0);
    const float v10 = randomValue(x1, z0);
    const float v01 = randomValue(x0, z1);
    const float v11 = randomValue(x1, z1);

    float tx = x - static_cast<float>(x0);
    float tz = z - static_cast<float>(z0);

    tx = smoothStep(tx);
    tz = smoothStep(tz);

    const float top = lerp(v00, v10, tx);
    const float bottom = lerp(v01, v11, tx);

    return lerp(top, bottom, tz);
}

bool Terrain::hasDifferentNeighbor(std::size_t x, std::size_t z) const
{
    const GrassType current = getCell(x, z).grassType;

    if (x + 1 < width_ && getCell(x + 1, z).grassType != current)
    {
        return true;
    }

    if (z + 1 < height_ && getCell(x, z + 1).grassType != current)
    {
        return true;
    }

    return false;
}

MeshData Terrain::createGrassMeshData() const
{
    MeshData data;

    const float terrainWidth = static_cast<float>(width_) * cellSize_;

    const float terrainDepth = static_cast<float>(height_) * cellSize_;

    const float startX = -terrainWidth * 0.5f;
    const float startZ = -terrainDepth * 0.5f;

    constexpr float sampleSpacing = 0.1f;
    constexpr float placementProbability = 0.15f;

    for (float z = startZ; z < startZ + terrainDepth - sampleSpacing; z += sampleSpacing)
    {
        for (float x = startX; x < startX + terrainWidth - sampleSpacing; x += sampleSpacing)
        {
            const int sampleX = static_cast<int>((x - startX) / sampleSpacing);
            const int sampleZ = static_cast<int>((z - startZ) / sampleSpacing);

            const GrassType here = getGrassTypeAt(x, z);
            const GrassType right = getGrassTypeAt(x + sampleSpacing, z);
            const GrassType forward = getGrassTypeAt(x, z + sampleSpacing);

            if (here != right)
            {
                const float chance = randomValue(sampleX + 700, sampleZ + 900);

                if (chance < placementProbability)
                {
                    Vec3 edgeCenter{x + sampleSpacing * 0.5f, 0.0f, z};

                    constexpr float sideOffset = 0.05f;
                    constexpr float maxJitter = 0.0f;

                    Vec3 hereCenter = edgeCenter;
                    Vec3 rightCenter = edgeCenter;

                    hereCenter.x -= sideOffset;
                    rightCenter.x += sideOffset;

                    const float jitterA = randomValue(sampleX + 1200, sampleZ + 1400) * 2.0f - 1.0f;
                    const float jitterB = randomValue(sampleX + 1500, sampleZ + 1600) * 2.0f - 1.0f;
                    
                    hereCenter.z += jitterA * maxJitter * sampleSpacing;
                    rightCenter.z += jitterB * maxJitter * sampleSpacing;

                    addGrassQuad(data, hereCenter, getGrassColor(here));
                    addGrassQuad(data, rightCenter, getGrassColor(right));
                }
            }

            if (here != forward)
            {
                const float chance = randomValue(sampleX + 1700, sampleZ + 1900);

                if (chance < placementProbability)
                {
                    Vec3 edgeCenter{x, 0.0f, z + sampleSpacing * 0.5f};

                    constexpr float sideOffset = 0.05f;
                    constexpr float maxJitter = 0.0f;

                    Vec3 hereCenter = edgeCenter;
                    Vec3 forwardCenter = edgeCenter;

                    hereCenter.z -= sideOffset;
                    forwardCenter.z += sideOffset;

                    const float jitterA = randomValue(sampleX + 2200, sampleZ + 2400) * 2.0f - 1.0f;
                    const float jitterB = randomValue(sampleX + 2500, sampleZ + 2700) * 2.0f - 1.0f;
                    
                    hereCenter.x += jitterA * maxJitter * sampleSpacing;
                    forwardCenter.x += jitterB * maxJitter * sampleSpacing;

                    addGrassQuad(data, hereCenter, getGrassColor(here));
                    addGrassQuad(data, forwardCenter, getGrassColor(forward));
                }  
            }
        }
    }

    return data;
}

void Terrain::addGrassQuad(MeshData& data, const Vec3& center, const Vec3& color) const
{
    const GLuint baseIndex = static_cast<GLuint>(data.vertices.size());

    data.vertices.push_back({center, color, {0.0f, 0.0f}, {-0.5f, 0.0f}});
    data.vertices.push_back({center, color, {1.0f, 0.0f}, {0.5f, 0.0f}});
    data.vertices.push_back({center, color, {1.0f, 1.0f}, {0.5f, 1.0f}});
    data.vertices.push_back({center, color, {0.0f, 1.0f}, {-0.5f, 1.0f}});

    data.indices.push_back(baseIndex + 0);
    data.indices.push_back(baseIndex + 1);
    data.indices.push_back(baseIndex + 2);

    data.indices.push_back(baseIndex + 0);
    data.indices.push_back(baseIndex + 2);
    data.indices.push_back(baseIndex + 3);
}

