#include <stb_image.h>
#include "ImageData.h"
#include "engine/assets/TextureLoader.h"
#include <filesystem>

ImageData TextureLoader::load(const std::filesystem::path& path)
{

    int width = 0;
    int height = 0;
    int channels = 0;

    stbi_set_flip_vertically_on_load(true);

    unsigned char* data = stbi_load(
        path.string().c_str(),
        &width,
        &height,
        &channels,
        4
    );

    if (data == nullptr)
    {
        throw std::runtime_error("Failed to load texture: " + path.string());
    }

    ImageData image;

    image.width = width;
    image.height = height;
    image.channels = 4;

    const std::size_t dataSize = static_cast<std::size_t>(width) * static_cast<std::size_t>(height) * 4;

    image.pixels.assign(data, data + dataSize);

    stbi_image_free(data);

    return image;
}