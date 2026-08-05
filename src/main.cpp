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

    //Create Scene
    {
        Scene scene;
        GameObjectID cube = scene.createGameObject();
        GameObjectID cubeB = scene.createGameObject();
        GameObjectID cubeC = scene.createGameObject();

        ShaderProgram shaderProgram("assets/shaders/basic.vert", "assets/shaders/basic.frag");
        //shaderProgram.use();

        Camera camera(Vec3{0.0f, 3.0f, 5.0f});
        //Camera camera;
        FreeFlyCameraController freeFlyController;
        CylinderCameraController cylinderController;


        CameraMode cameraMode = CameraMode::FreeFly;
        CameraController* activeController = &cylinderController;

        activeController -> activate(window, camera);

        MeshData cubeData {PrimitiveMesh::getCubeVertices(), PrimitiveMesh::getCubeIndices()};
        Mesh cubeMesh{cubeData};
        scene.getGameObject(cube).addRenderComponent(cubeMesh, shaderProgram);
        scene.getGameObject(cubeB).addRenderComponent(cubeMesh, shaderProgram);
        scene.getGameObject(cubeC).addRenderComponent(cubeMesh, shaderProgram);
        Renderer renderer;
    
        bool previousCPressed = false;

        float lastFrameTime = static_cast<float>(glfwGetTime());

        //Main loop
        while (glfwWindowShouldClose(window) == GLFW_FALSE)
        {
            glfwPollEvents();

            const float currentTime = static_cast<float>(glfwGetTime());
            const float deltaTime = currentTime - lastFrameTime;
            lastFrameTime = currentTime;

            const bool cPressed = glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS;

            if (cPressed && !previousCPressed)
            {
                if (activeController == &freeFlyController)
                {
                    activeController = &cylinderController;
                }

                else if (activeController == &cylinderController)
                {
                    activeController = &freeFlyController;
                }

                activeController -> activate(window, camera);
            }
            previousCPressed = cPressed;
            activeController -> update(window, camera, deltaTime);

            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            {
                glfwSetWindowShouldClose(window, GLFW_TRUE);
            }

            glClearColor(0.08F, 0.10F, 0.14F, 1.0F);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
            float time = glfwGetTime();
            float sinTime = sin(time);
       
            //scene.getGameObject(cube).getTransform().rotate({0.0f, time * 50.0f, 0.0f});
            //scene.getGameObject(cube).getTransform().setPosition({sinTime, 0.0f, 0.0f});
            scene.getGameObject(cubeB).getTransform().setPosition({4.0f, 0.0f, 3.0f});
            scene.getGameObject(cubeC).getTransform().setPosition({-4.0f, 0.0f, 2.5f});
            renderer.render(scene, camera);


            glfwSwapBuffers(window);
            
        }
    }
    glfwDestroyWindow(window);
    glfwTerminate();

    return EXIT_SUCCESS;
}