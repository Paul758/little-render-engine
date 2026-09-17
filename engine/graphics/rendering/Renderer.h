#pragma once

class RenderSystem;
class CameraSystem;
class LightingSystem;
struct RenderViewport;


class Renderer
{
public:
    Renderer(RenderSystem& renderSystem, CameraSystem& cameraSystem, LightingSystem& lightSystem);
    void renderViewport(const RenderViewport& viewport);
private:
    RenderSystem& renderSystem_;
    CameraSystem& cameraSystem_;
    LightingSystem& lightSystem_;
};