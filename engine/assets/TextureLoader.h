#pragma once

#include "ImageData.h"
#include <filesystem>

class TextureLoader
{
public:
    static ImageData load(const std::filesystem::path& path);
};