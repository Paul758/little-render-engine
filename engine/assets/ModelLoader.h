#pragma once

#include <filesystem>
#include "engine/assets/ModelData.h"

class ModelLoader
{
public:
    static ModelData load(const std::filesystem::path& path);

private:

};

