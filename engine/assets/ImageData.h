#pragma once

#include <cstdint>
#include <vector>

struct ImageData
{
    int width = 0;
    int height = 0;
    int channels = 0;

    std::vector<std::uint8_t> pixels;
};