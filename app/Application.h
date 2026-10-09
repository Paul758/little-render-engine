#pragma once

#include <memory>

#include "engine/serialization/ComponentSerializationRegistry.h"
#include "engine/scene/SceneService.h"

struct GLFWwindow;

class Input;
class GameTime;
class Game;

class RenderSystem;
class CameraSystem;
class LightingSystem;
class Renderer;

class Editor;
class GameEditor;

class Framebuffer;
class ScreenRenderer;

class ShaderProgram;
class AssetManager;

class SceneService;
class SceneSerializer;
class ComponentSerializationRegistry;

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
    std::unique_ptr<ScreenRenderer> screenRenderer_;

    std::unique_ptr<Editor> editor_;
    std::unique_ptr<GameEditor> gameEditor_;
    
    std::unique_ptr<AssetManager> assetManager_;

    // Scene loading
    std::unique_ptr<ComponentSerializationRegistry> registry_;
    std::unique_ptr<SceneService> sceneService_;
};