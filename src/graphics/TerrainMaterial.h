#include "graphics/Material.h"

class TerrainMaterial : public Material
{
private:
    ShaderProgram* shader_;

public:
    explicit TerrainMaterial(ShaderProgram& shader);

    const ShaderProgram& getShader() const override;

    void apply(const Camera& camera, const GameObject& gameObject) const override;
};