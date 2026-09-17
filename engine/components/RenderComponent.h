#pragma once

#include "engine/graphics/Mesh.h"
#include "engine/graphics/ShaderProgram.h"
#include "engine/graphics/materials/Material.h"

struct RenderComponent
{
    const Mesh* mesh = nullptr;
    const Material* material = nullptr;
};