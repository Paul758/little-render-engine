#include "app/Application.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <memory>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "engine/core/input/Input.h"
#include "engine/core/time/GameTime.h"
#include "game/Game.h"
#include "game-editor/GameEditor.h"

#include "engine/systems/RenderSystem.h"
#include "engine/systems/CameraSystem.h"
#include "engine/systems/LightingSystem.h"
#include "engine/graphics/rendering/Renderer.h"

#include "engine/serialization/EngineSerialization.h"
#include "game/serialization/GameSerialization.h"

#include "editor/Editor.h"

#include "engine/graphics/Framebuffer.h"
#include "engine/graphics/rendering/ScreenRenderer.h"

#include "engine/graphics/ShaderProgram.h"

#include "engine/assets/AssetManager.h"

Application::Application() = default;

bool Application::initialize()
{
     if (glfwInit() != GLFW_TRUE)
    {
        std::cerr << "Failed to initialize GLFW\n";
        return false;
    }

    // Request an OpenGL 3.3 Core context.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    window_ = glfwCreateWindow(
        1280,
        720,
        "Little Renderer",
        nullptr,
        nullptr
    );

    if (window_ == nullptr)
    {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window_);

    const int version = gladLoadGL(glfwGetProcAddress);

    if (version == 0)
    {
        std::cerr << "Failed to initialize GLAD\n";
        glfwDestroyWindow(window_);
        window_ = nullptr;
        glfwTerminate();

        return false;
    }

    std::cout
        << "Loaded OpenGL "
        << GLAD_VERSION_MAJOR(version)
        << '.'
        << GLAD_VERSION_MINOR(version)
        << '\n';

    glfwSwapInterval(1);

    //enable depth testing
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_MULTISAMPLE);

    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window_, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // Assets
    assetManager_ = std::make_unique<AssetManager>("assets");

    //Scene
    registry_ = std::make_unique<ComponentSerializationRegistry>();

    // Initialize Serialization
    EngineSerialization::registerComponents(*registry_);
    GameSerialization::registerComponents(*registry_);

    sceneService_ = std::make_unique<SceneService>(*registry_);



    // Initialize Input and Time
    input_ = std::make_unique<Input>(window_);
    time_ = std::make_unique<GameTime>();
    game_ = std::make_unique<Game>(*assetManager_);
    game_->initialize();

    editor_ = std::make_unique<Editor>(game_->getWorld(), game_->getHiearchySystem());

    gameEditor_ = std::make_unique<GameEditor>();
    gameEditor_->registerComponentEditors(editor_->getComponentEditorRegistry());

    editor_->initialize();

    ComponentRegistry& components = game_->getWorld().components();
    renderSystem_ = std::make_unique<RenderSystem>(components);
    cameraSystem_ = std::make_unique<CameraSystem>(components);
    lightingSystem_ = std::make_unique<LightingSystem>(components);
    renderer_ = std::make_unique<Renderer>(*renderSystem_, *cameraSystem_, *lightingSystem_);

    editorFramebuffer_ = std::make_unique<Framebuffer>(640, 720, TextureFilter::Linear);
    gameFramebuffer_ = std::make_unique<Framebuffer>(640, 720, TextureFilter::Linear);

    ShaderProgram& screenShader = assetManager_->loadShader("shaders/screen.vert", "shaders/screen.frag");
    screenRenderer_ = std::make_unique<ScreenRenderer>(screenShader);

    return true;
}

void Application::run()
{
    while(glfwWindowShouldClose(window_) == GLFW_FALSE)
    {
        // Input
        input_->beginFrame();
        glfwPollEvents();

        // Update
        time_->update();
        input_->update();

        // ImGui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        editor_->drawGui();

        // Game and editor update
        game_->update(*input_, time_->deltaTime());
        editor_->update(*input_, time_->deltaTime());

        if (glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window_, GLFW_TRUE);
        }

        RenderViewport editorViewport{
            editor_->getCamera(),
            editorFramebuffer_->getWidth(),
            editorFramebuffer_->getHeight(),
            editorFramebuffer_.get()
        };

        RenderViewport gameViewport{
            game_->getCamera(),
            gameFramebuffer_->getWidth(),
            gameFramebuffer_->getHeight(),
            gameFramebuffer_.get()
        };

        renderer_->renderViewport(editorViewport);
        renderer_->renderViewport(gameViewport);
    
        Framebuffer::unbind();

        int width = 0;
        int height = 0;

        glfwGetFramebufferSize(window_, &width, &height);

        if (width > 0 && height > 0)
        {
            glDisable(GL_DEPTH_TEST);

             glViewport(0, 0, width, height);

            glClear(GL_COLOR_BUFFER_BIT);

            int halfWidth = width / 2;

            screenRenderer_->drawTexture(
                editorFramebuffer_->getColorTexture(),
                0,
                0,
                halfWidth,
                height
            );

            screenRenderer_->drawTexture(
                gameFramebuffer_->getColorTexture(),
                halfWidth,
                0,
                width - halfWidth,
                height
            );

            glEnable(GL_DEPTH_TEST);
        }     

        // ImGui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window_);
    }
}

Application::~Application()
{
    screenRenderer_.reset();

    editorFramebuffer_.reset();
    gameFramebuffer_.reset();

    renderer_.reset();
    lightingSystem_.reset();
    cameraSystem_.reset();
    renderSystem_.reset();

    editor_.reset();
    game_.reset();
    
    assetManager_.reset();

    input_.reset();
    time_.reset();

    sceneService_.reset();
    registry_.reset();

    // ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    if (window_ != nullptr)
    {
        glfwDestroyWindow(window_);
        window_ = nullptr;
    }

    glfwTerminate();
}