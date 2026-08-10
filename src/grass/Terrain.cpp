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
            const float value = samplePatchValue(x, z);

            if (value < 0.35f)
            {
                getCell(x, z).grassType = GrassType::Dark;
            }
            else if (value < 0.65f)
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

            const Vec3 color = getGrassColor(cell.grassType);

            
            const float worldX = startX + static_cast<float>(x) * cellSize_;
            const float worldZ = startZ + static_cast<float>(z) * cellSize_;

            const Vec3 p0 {worldX, 0.0f, worldZ};
            const Vec3 p1 {worldX + cellSize_, 0.0f, worldZ};
            const Vec3 p3 {worldX + cellSize_, 0.0f, worldZ + cellSize_};
            const Vec3 p2 {worldX, 0.0f, worldZ + cellSize_};

            const GLuint baseIndex = static_cast<GLuint> (data.vertices.size());

            data.vertices.push_back({p0, color});
            data.vertices.push_back({p1, color});
            data.vertices.push_back({p2, color});
            data.vertices.push_back({p3, color});

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

    for (std::size_t z = 0; z < height_; ++z)
    {
        for (std::size_t x = 0; x < width_; ++x)
        {
            const GrassType current = getCell(x, z).grassType;

            const float worldX = startX + static_cast<float>(x) * cellSize_;
            const float worldZ = startZ + static_cast<float>(z) * cellSize_;

            if (x + 1 < width_)
            {
                const GrassType right = getCell(x + 1, z).grassType;

                if (current != right)
                {
                    
                    const float placementChance = randomValue(static_cast<int>(x) + 700, static_cast<int>(z) + 900);

                    if (placementChance < 0.3f)
                    {
                        Vec3 edgeCenter{worldX + cellSize_, 0.0f, worldZ + cellSize_ * 0.5f};
                        constexpr float sideOffset = 0.08f;

                        Vec3 currentCenter = edgeCenter;
                        Vec3 rightCenter = edgeCenter;

                        currentCenter.x -= sideOffset;
                        rightCenter.x += sideOffset;

                        const float jitterA = randomValue(static_cast<int>(x) + 1200, static_cast<int>(z) + 1400) * 2.0f - 1.0f;
                        const float jitterB = randomValue(static_cast<int>(x) + 1500, static_cast<int>(z) + 1600) * 2.0f - 1.0f;

                        constexpr float maxJitter = 0.3f;

                        currentCenter.z += jitterA * maxJitter * cellSize_;
                        rightCenter.z += jitterB * maxJitter * cellSize_;
                        
                        addGrassQuad(data, currentCenter, getGrassColor(current));
                        addGrassQuad(data, rightCenter, getGrassColor(right));
                    }
                }
            }

            if (z + 1 < height_)
            {
                const GrassType forward = getCell(x, z + 1).grassType;

                if (current != forward)
                {
                    const float placementChance = randomValue(static_cast<int>(x) + 1700, static_cast<int>(z) + 1900);

                    if (placementChance < 0.3f)
                    {
                        Vec3 edgeCenter{worldX + cellSize_ * 0.5f, 0.0f, worldZ + cellSize_};

                        constexpr float sideOffset = 0.08f;

                        Vec3 currentCenter = edgeCenter;
                        Vec3 forwardCenter = edgeCenter;

                        currentCenter.z -= sideOffset;
                        forwardCenter.z += sideOffset;

                        const float jitterA = randomValue(static_cast<int>(x) + 2200, static_cast<int>(z) + 2400) * 2.0f - 1.0f;
                        const float jitterB = randomValue(static_cast<int>(x) + 2500, static_cast<int>(z) + 2700) * 2.0f - 1.0f;

                        constexpr float maxJitter = 0.3f;

                        currentCenter.x += jitterA * maxJitter * cellSize_;
                        forwardCenter.x += jitterB * maxJitter * cellSize_;

                        addGrassQuad(data, currentCenter, getGrassColor(current));
                        addGrassQuad(data, forwardCenter, getGrassColor(forward));
                    }
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