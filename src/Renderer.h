#pragma once

class RenderSystem;
class CameraSystem;
struct RenderViewport;


class Renderer
{
public:
    Renderer(RenderSystem& renderSystem, CameraSystem& cameraSystem);
    void renderViewport(const RenderViewport& viewport);
private:
    RenderSystem& renderSystem_;
    CameraSystem& cameraSystem_;
};