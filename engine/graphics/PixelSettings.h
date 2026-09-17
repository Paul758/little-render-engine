#pragma once

struct PixelSettings
{
    static constexpr int renderWidth = 640;
    static constexpr int renderHeight = 360;

    static constexpr float pixelsPerWorldUnit = 16.0f;

    static constexpr float orthographicSize()
    {
        return static_cast<float>(renderHeight) / (2.0f * pixelsPerWorldUnit);
    }

    static constexpr float worldUnitsPerPixel()
    {
        return 1.0f / pixelsPerWorldUnit;
    }
};