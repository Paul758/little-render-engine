#include "Mesh.h"
#include "graphics/ShaderProgram.h"

struct RenderComponent
{
    const Mesh* mesh = nullptr;
    const ShaderProgram* shader = nullptr;
};