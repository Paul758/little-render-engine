#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <cstdlib>
#include <iostream>
#include <filesystem>
#include <vector>

#include "graphics/ShaderProgram.h"
#include "Mesh.h"
#include "PrimitiveMesh.h"
#include "Camera.h"
#include "math/MathUtils.h"
#include "GameObject.h"
#include "FreeFlyCameraController.h"
#include "CylinderCameraController.h"
#include "CameraMode.h"
#include "PixelFramebuffer.h"
#include "OrbitCameraController.h"
#include "PlayerController.h"
#include "grass/Terrain.h"
#include "graphics/Texture2D.h"
#include "graphics/materials/BasicMaterial.h"
#include "graphics/materials/GrassMaterial.h"
#include "graphics/materials/TerrainMaterial.h"
#include "graphics/RenderViewport.h"
#include "PixelRenderer.h"
#include "GameTime.h"
#include "Renderer.h"

#include "components/PlayerInputComponent.h"
#include "components/TransformComponent.h"
#include "components/RenderComponent.h"
#include "components/LocomotionIntentComponent.h"
#include "components/BehaviourTreeComponent.h"
#include "components/VelocityComponent.h"
#include "components/camera/CameraComponent.h"
#include "components/camera/FreeFlyCameraComponent.h"
#include "components/camera/FreeHandCameraComponent.h"
#include "components/camera/WorldCameraComponent.h"
#include "components/camera/FixedOrbitCameraComponent.h"
#include "components/camera/OrbitPose.h"

#include "systems/RenderSystem.h"
#include "systems/InputSystem.h"
#include "systems/IntentResetSystem.h"
#include "systems/LocomotionSystem.h"
#include "systems/ActionResolver.h"
#include "systems/CommandBuffer.h"
#include "systems/IntegrationSystem.h"
#include "systems/BehaviourTreeSystem.h"
#include "systems/CameraSystem.h"
#include "systems/EditorCameraUpdateSystem.h"
#include "systems/EditorSystem.h"
#include "systems/FixedOrbitCameraSystem.h"

#include "behaviour/player/PlayerMovementBTreeBuilder.h"
#include "behaviour/BehaviourContext.h"
#include "behaviour/BehaviourTreeRegistry.h"

#include "ecs/World.h"
#include "Scene.h"
#include "ScreenRenderer.h"


namespace
{
void framebuffer_size_callback(
    GLFWwindow* /*window*/,
    int width,
    int height
)
{
    glViewport(0, 0, width, height);
}
}

int main()
{
    if (glfwInit() != GLFW_TRUE)
    {
        std::cerr << "Failed to initialize GLFW\n";
        return EXIT_FAILURE;
    }

    // Request an OpenGL 3.3 Core context.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    GLFWwindow* window = glfwCreateWindow(
        1280,
        720,
        "Little Renderer",
        nullptr,
        nullptr
    );

    if (window == nullptr)
    {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    const int version = gladLoadGL(glfwGetProcAddress);

    if (version == 0)
    {
        std::cerr << "Failed to initialize GLAD\n";
        glfwDestroyWindow(window);
        glfwTerminate();
        return EXIT_FAILURE;
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

    //Create Scene
    {
        World world;

        Entity cube = world.createEntity();
        Entity cubeB = world.createEntity();
        Entity cubeC = world.createEntity();

        TransformComponent transformCube;
        TransformComponent transformCubeB;
        TransformComponent transformCubeC;

        //Texture
        Texture2D grassTexture("assets/textures/grass-small-3.png");
        grassTexture.bind(0);

        //Shader
        ShaderProgram basicShader("assets/shaders/basic.vert", "assets/shaders/basic.frag");
        ShaderProgram grassShader("assets/shaders/grass.vert", "assets/shaders/grass.frag");
        ShaderProgram terrainShader("assets/shaders/terrain.vert", "assets/shaders/terrain.frag");
        ShaderProgram postProcessShader("assets/shaders/postprocess.vert", "assets/shaders/postprocess.frag");

        ShaderProgram screenShader("assets/shaders/screen.vert", "assets/shaders/screen.frag");

        ScreenRenderer screenRenderer(screenShader);

        BasicMaterial basicMaterial(basicShader);
        GrassMaterial grassMaterial(grassShader, grassTexture);
        TerrainMaterial terrainMaterial(terrainShader);

        MeshData cubeData {PrimitiveMesh::getCubeVertices(), PrimitiveMesh::getCubeIndices()};
        Mesh cubeMesh{cubeData};

        RenderComponent renderComponentCube{&cubeMesh, &basicMaterial};
        RenderComponent renderComponentCubeB{&cubeMesh, &basicMaterial};
        RenderComponent renderComponentCubeC{&cubeMesh, &basicMaterial};

        //Camera camera(Vec3{0.0f, 3.0f, 5.0f});
        //FreeFlyCameraController freeFlyController;
        //CylinderCameraController cylinderController;
        //OrbitCameraController orbitCameraController;

        //CameraController* activeController = &cylinderController;

        //activeController -> activate(window, camera);

        world.components().add(cube, transformCube);
        world.components().add(cubeB, transformCubeB);
        world.components().add(cubeC, transformCubeC);

        world.components().add(cube, renderComponentCube);
        world.components().add(cubeB, renderComponentCubeB);
        world.components().add(cubeC, renderComponentCubeC);

        RenderSystem renderSystem(world.components());
        
        InputSystem inputSystem(world.components());

        PlayerMovementBTreeBuilder playerTreeBuilder;
        auto playerLocomotionTree = playerTreeBuilder.build();
        BehaviourTreeRegistry treeRegistry;
        BehaviourTreeId playerTreeId = treeRegistry.add(std::move(playerLocomotionTree));

        BehaviourTreeSystem behaviourTreeSystem(world.components(), treeRegistry);
        IntentResetSystem intentResetSystem(world.components());
        LocomotionSystem locomotionSystem(world.components());
        ActionResolver actionResolver(world.components());
        CommandBuffer commandBuffer;
        IntegrationSystem integrationSystem(world.components());

        Input input(window);
        

        BehaviourContext behaviourContext{cube, world.components()};

        world.components().add(cube, PlayerInputComponent{});
        world.components().add(cube, LocomotionIntentComponent{});
        world.components().add(cube, BehaviourTreeComponent{playerTreeId});
        world.components().add(cube, VelocityComponent{});

        //Setup editor camera
        Entity editorCameraEntity = world.createEntity();
        CameraSystem cameraSystem(world.components());
        TransformComponent editorTransform;
        editorTransform.position = Vec3{0.0f, 2.0f, 5.0f};
        editorTransform.rotation = Quaternion::lookRotation(Vec3{0.0f, 0.0f, 0.0f} - editorTransform.position, Vec3{0.0f, 1.0f, 0.0f});


        world.components().add(editorCameraEntity, editorTransform);
        world.components().add(editorCameraEntity, CameraComponent{});
        world.components().add(editorCameraEntity, FreeFlyCameraComponent{});
        world.components().add(editorCameraEntity, FreeHandCameraComponent{});
        world.components().add(editorCameraEntity, WorldCameraComponent{});

        EditorCameraUpdateSystem editorCameraUpdateSystem(world.components(), editorCameraEntity);
        EditorSystem editorSystem(world.components(), editorCameraEntity);

        //Setting up Game camera
        Entity gameCameraEntity = world.createEntity();
        std::vector<OrbitPose> orbitPoses{{45.0f, 30.0f}, {135.0f, 30.0f}, {225.0f, 30.0f}, {315.0f, 30.f}};
        FixedOrbitCameraComponent fixedOrbitCameraComponent{transformCube.position, orbitPoses, 0, 8.0f, 10.0f};
        world.components().add(gameCameraEntity, TransformComponent{});
        world.components().add(gameCameraEntity, CameraComponent{});
        world.components().add(gameCameraEntity, WorldCameraComponent{});
        world.components().add(gameCameraEntity, fixedOrbitCameraComponent);

        FixedOrbitCameraSystem fixedOrbitCameraSystem(world.components());
        fixedOrbitCameraSystem.initialize(gameCameraEntity);

        Framebuffer editorFramebuffer {640, 720, TextureFilter::Linear};
        Framebuffer gameFramebuffer {640, 720, TextureFilter::Linear};

        RenderViewport editorViewport{
            .camera = editorCameraEntity,
            .width = editorFramebuffer.getWidth(),
            .height = editorFramebuffer.getHeight(),
            .target = &editorFramebuffer
        };

        RenderViewport gameViewport{
            .camera = gameCameraEntity,
            .width = gameFramebuffer.getWidth(),
            .height = gameFramebuffer.getHeight(),
            .target = &gameFramebuffer
        };

        Renderer renderer(renderSystem, cameraSystem);

        //Pixel screen
        //PixelRenderer pixelRenderer(640, 360, postProcessShader);

        GameTime time;

        //Main loop
        while (glfwWindowShouldClose(window) == GLFW_FALSE)
        {
            input.beginFrame();
            glfwPollEvents();

            time.update();
            input.update();

            inputSystem.update(input);
            intentResetSystem.update();
            commandBuffer.clear();
            behaviourTreeSystem.update();
            actionResolver.update(commandBuffer);
            locomotionSystem.update(commandBuffer);
            integrationSystem.update(time.deltaTime());

            editorSystem.update(input, time.deltaTime());
            fixedOrbitCameraSystem.update(input, gameCameraEntity, time.deltaTime());

            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            {
                glfwSetWindowShouldClose(window, GLFW_TRUE);
            }

            //RenderViewport renderViewport{editorCameraEntity, 1280, 720};
            //RenderView renderView = cameraSystem.buildRenderView(renderViewport);

            //pixelRenderer.beginFrame();
            //renderSystem.render(renderView);
            //pixelRenderer.present(window);
            
            renderer.renderViewport(editorViewport);
            renderer.renderViewport(gameViewport);

            Framebuffer::unbind();

            int windowWidth = 0;
            int windowHeight = 0;
            glfwGetFramebufferSize(window, &windowWidth, &windowHeight);

            glDisable(GL_DEPTH_TEST);

            glViewport(0, 0, windowWidth, windowHeight);

            glClear(GL_COLOR_BUFFER_BIT);

            const int halfWidth = windowWidth / 2;

            screenRenderer.drawTexture(editorFramebuffer.getColorTexture(), 0, 0, halfWidth, windowHeight);
            screenRenderer.drawTexture(gameFramebuffer.getColorTexture(), halfWidth, 0, windowWidth - halfWidth, windowHeight);

            glEnable(GL_DEPTH_TEST);

            glfwSwapBuffers(window);
            
        }
    }
    glfwDestroyWindow(window);
    glfwTerminate();

    return EXIT_SUCCESS;
}