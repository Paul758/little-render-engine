#pragma once

#include "Mesh.h"
#include "graphics/ShaderProgram.h"
#include "graphics/materials/Material.h"

struct RenderComponent
{
    const Mesh* mesh = nullptr;
    const Material* material = nullptr;
};