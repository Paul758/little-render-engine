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
#include "PixelRenderer.h"
#include "GameTime.h"

#include "systems/RenderSystem.h"
#include "ecs/World.h"
#include "Scene.h"

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
        1920,
        1080,
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

        BasicMaterial basicMaterial(basicShader);
        GrassMaterial grassMaterial(grassShader, grassTexture);
        TerrainMaterial terrainMaterial(terrainShader);

        MeshData cubeData {PrimitiveMesh::getCubeVertices(), PrimitiveMesh::getCubeIndices()};
        Mesh cubeMesh{cubeData};

        RenderComponent renderComponentCube{&cubeMesh, &basicMaterial};
        RenderComponent renderComponentCubeB{&cubeMesh, &basicMaterial};
        RenderComponent renderComponentCubeC{&cubeMesh, &basicMaterial};

        Camera camera(Vec3{0.0f, 3.0f, 5.0f});
        FreeFlyCameraController freeFlyController;
        CylinderCameraController cylinderController;
        OrbitCameraController orbitCameraController;

        CameraController* activeController = &cylinderController;

        activeController -> activate(window, camera);

        world.components().add(cube, transformCube);
        world.components().add(cube, transformCubeB);
        world.components().add(cube, transformCubeC);

        world.components().add(cube, renderComponentCube);
        world.components().add(cube, renderComponentCubeB);
        world.components().add(cube, renderComponentCubeC);

        //Renderer renderer;
        RenderSystem renderSystem(world.components());

        PlayerController playerController(orbitCameraController);
        Input input(window);
        
        //Pixel screen
        PixelRenderer pixelRenderer(640, 360, postProcessShader);

        bool previousCPressed = false;

        GameTime time;

        //Main loop
        while (glfwWindowShouldClose(window) == GLFW_FALSE)
        {
            glfwPollEvents();

            time.update();

            const bool cPressed = glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS;

            if (cPressed && !previousCPressed)
            {
                if (activeController == &freeFlyController)
                {
                    activeController = &orbitCameraController;
                }

                else if (activeController == &cylinderController)
                {
                    activeController = &freeFlyController;
                }

                activeController -> activate(window, camera);
            }
            previousCPressed = cPressed;
            activeController -> update(window, camera, time.deltaTime());

            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            {
                glfwSetWindowShouldClose(window, GLFW_TRUE);
            }

            RenderView renderView{
                camera.getViewMatrix(),
                camera.getProjectionMatrix(),
                camera.getRight(),
                camera.getUp()
            };

            pixelRenderer.beginFrame();
            renderSystem.render(renderView);
            pixelRenderer.present(window);
        
            glfwSwapBuffers(window);
            
        }
    }
    glfwDestroyWindow(window);
    glfwTerminate();

    return EXIT_SUCCESS;
}