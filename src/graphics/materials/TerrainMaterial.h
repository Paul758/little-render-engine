#include "graphics/materials/Material.h"

class TerrainMaterial : public Material
{
private:
    ShaderProgram* shader_;

public:
    explicit TerrainMaterial(ShaderProgram& shader);

    const ShaderProgram& getShader() const override;

    void apply(const RenderContext& renderContext) const override;
};