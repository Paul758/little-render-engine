#pragma once

#include <memory>

struct GLFWwindow;

class Input;
class GameTime;
class Game;

class RenderSystem;
class CameraSystem;
class LightingSystem;
class Renderer;

class Editor;

class Framebuffer;
class ScreenRenderer;

class ShaderProgram;

class Application
{
public:
    Application();
    ~Application();

    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    bool initialize();
    void run();

private:
    GLFWwindow* window_ = nullptr;

    std::unique_ptr<Input> input_;
    std::unique_ptr<GameTime> time_;
    std::unique_ptr<Game> game_;

    std::unique_ptr<RenderSystem> renderSystem_;
    std::unique_ptr<CameraSystem> cameraSystem_;
    std::unique_ptr<LightingSystem> lightingSystem_;
    std::unique_ptr<Renderer> renderer_;

    std::unique_ptr<Framebuffer> editorFramebuffer_;
    std::unique_ptr<Framebuffer> gameFramebuffer_;
    std::unique_ptr<ShaderProgram> screenShader_;
    std::unique_ptr<ScreenRenderer> screenRenderer_;

    std::unique_ptr<Editor> editor_;


};