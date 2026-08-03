#include <iostream>

#include "Renderer.h"

GameObject& Renderer::addObject(GameObject&& gameObject)
{
    gameObjects.push_back(std::move(gameObject));
    return gameObjects.back();
}


void Renderer::render(Scene& scene, Camera& camera) const
{
    for (const GameObject& object : scene.getGameObjects())
    {
        const RenderComponent* renderComponent = object.getRenderComponent();

        if (renderComponent == nullptr || renderComponent -> mesh == nullptr || renderComponent -> shader == nullptr)
        {
            continue;
        }

        const ShaderProgram* shader = object.getRenderComponent()->shader;

        const Mesh* mesh = object.getRenderComponent()->mesh;

        shader -> use();
        Mat4 result =  object.getTransform().getModelMatrix();
        /*std::cout << "resulting matrix in renderer is \n" << result.at(0,0) << result.at(0,1) << result.at(0,2) << result.at(0,3) << "\n"
                                << result.at(1,0) << result.at(1,1) << result.at(1,2) << result.at(1,3) << "\n"
                                << result.at(2,0) << result.at(2,1) << result.at(2,2) << result.at(2,3) << "\n"
                                << result.at(3,0) << result.at(3,1) << result.at(3,2) << result.at(3,3) << "\n";*/
        shader -> setMat4("model", object.getTransform().getModelMatrix());
        shader -> setMat4("view", camera.getViewMatrix());
        shader -> setMat4("projection", camera.getProjectionMatrix());

        mesh->draw();
    }
}
