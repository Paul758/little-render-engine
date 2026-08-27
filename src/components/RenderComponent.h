#include "Mesh.h"
#include "graphics/ShaderProgram.h"
#include "graphics/Material.h"

struct RenderComponent
{
    const Mesh* mesh = nullptr;
    const Material* material = nullptr;
};