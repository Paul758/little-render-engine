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
#include "Renderer.h"
#include "FreeFlyCameraController.h"
#include "CylinderCameraController.h"
#include "CameraMode.h"
#include "PixelFramebuffer.h"
#include "OrbitCameraController.h"
#include "PlayerController.h"
#include "grass/Terrain.h"
#include "graphics/Texture2D.h"
#include "graphics/BasicMaterial.h"
#include "graphics/GrassMaterial.h"
#include "graphics/TerrainMaterial.h"
#include "PixelRenderer.h"
#include "GameTime.h"
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
        Scene scene;
        GameObjectID cube = scene.createGameObject();
        GameObjectID cubeB = scene.createGameObject();
        GameObjectID cubeC = scene.createGameObject();

        //Texture
        Texture2D grassTexture("assets/textures/grass-small-3.png");
        grassTexture.bind(0);

        ShaderProgram basicShader("assets/shaders/basic.vert", "assets/shaders/basic.frag");
        ShaderProgram grassShader("assets/shaders/grass.vert", "assets/shaders/grass.frag");
        ShaderProgram terrainShader("assets/shaders/terrain.vert", "assets/shaders/terrain.frag");
        ShaderProgram postProcessShader("assets/shaders/postprocess.vert", "assets/shaders/postprocess.frag");

        BasicMaterial basicMaterial(basicShader);
        GrassMaterial grassMaterial(grassShader, grassTexture);
        TerrainMaterial terrainMaterial(terrainShader);

        Camera camera(Vec3{0.0f, 3.0f, 5.0f});
        FreeFlyCameraController freeFlyController;
        CylinderCameraController cylinderController;
        OrbitCameraController orbitCameraController;

        CameraController* activeController = &cylinderController;

        activeController -> activate(window, camera);

        MeshData cubeData {PrimitiveMesh::getCubeVertices(), PrimitiveMesh::getCubeIndices()};
        Mesh cubeMesh{cubeData};
        scene.getGameObject(cube).addRenderComponent(cubeMesh, basicMaterial);
        scene.getGameObject(cubeB).addRenderComponent(cubeMesh, basicMaterial);
        scene.getGameObject(cubeC).addRenderComponent(cubeMesh, basicMaterial);
        Renderer renderer;
        
        PlayerController playerController(orbitCameraController);
        Input input(window);
        
        //Pixel screen
        PixelRenderer pixelRenderer(640, 360, postProcessShader);

        //Create Terrain
        Terrain terrain(512, 512, 0.2f);
        terrain.generateTerrain();
        MeshData terrainData = terrain.createMeshData();
        Mesh terrainMesh{terrainData};
        GameObjectID terrainObject = scene.createGameObject();
        scene.getGameObject(terrainObject).addRenderComponent(terrainMesh, terrainMaterial);

        //Create Grass
        
        MeshData grassData = terrain.createGrassMeshData();
        Mesh grassMesh{grassData};
        GameObjectID grassObject = scene.createGameObject();
        scene.getGameObject(grassObject).addRenderComponent(grassMesh, grassMaterial);

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

            
            //float time = glfwGetTime();
            //float sinTime = sin(time);
       
            //scene.getGameObject(cube).getTransform().rotate({0.0f, time * 50.0f, 0.0f});
            //scene.getGameObject(cube).getTransform().setPosition({sinTime, 0.0f, 0.0f});
            Vec3 cubePosition = scene.getGameObject(cube).getTransform().getPosition();
            scene.getGameObject(cube).getTransform().setPosition({cubePosition.x, 0.5f, cubePosition.z});
            scene.getGameObject(cubeB).getTransform().setPosition({4.0f, 0.0f, 3.0f});
            scene.getGameObject(cubeC).getTransform().setPosition({-4.0f, 0.0f, 2.0f});

            playerController.update(input, scene.getGameObject(cube), time.deltaTime());

            pixelRenderer.beginFrame();
            renderer.render(scene, camera);
            pixelRenderer.present(window);
        
            glfwSwapBuffers(window);
            
        }
    }
    glfwDestroyWindow(window);
    glfwTerminate();

    return EXIT_SUCCESS;
}