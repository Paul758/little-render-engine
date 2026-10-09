#include "engine/scene/SceneFile.h"
#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>

void SceneFile::write(const std::filesystem::path& path, const std::string& contents)
{
    std::ofstream file(path, std::ios::binary | std::ios::trunc);

    if (!file)
    {
            const int error = errno;
            throw std::system_error(error, std::generic_category(), "Could not open scene file for writing: " + path.string());
            //throw std::runtime_error("Could not open scene file for writing: " + path.string());
    }

    file.write(contents.data(), static_cast<std::streamsize>(contents.size()));

    if (!file)
    {
        throw std::runtime_error("Failed to write scene file: " + path.string());
    }

    file.close();

    if (!file)
    {
        throw std::runtime_error("Failed to close scene file after writing: " + path.string());
    }
}

std::string SceneFile::read(const std::filesystem::path& path)
{
    std::ifstream file(path, std::ios::binary);

    if (!file)
    {
        throw std::runtime_error("Could not open scene file for reading: " + path.string());
    }

    std::ostringstream buffer;
    buffer << file.rdbuf();

    if (file.bad())
    {
        throw std::runtime_error("Failed to read scene file: " + path.string());
    }

    if (!buffer)
    {
        throw std::runtime_error("Failed to buffer scene file: " + path.string());
    }
    return buffer.str();
}
