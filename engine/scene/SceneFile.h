#pragma once
#include <filesystem>

class SceneFile
{
public:
    static void write(const std::filesystem::path& path, const std::string& contents);
    static std::string read(const std::filesystem::path& path);

private:
};