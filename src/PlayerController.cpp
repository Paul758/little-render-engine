#include<iostream>

#include "PlayerController.h"


PlayerController::PlayerController(OrbitCameraController& cameraController) : cameraController_(cameraController)
{
}

void PlayerController::update(Input& input, GameObject& gameObject, float deltaTime)
{
    forward_ = cameraController_.getForward();
    right_ = cameraController_.getRight();
    

    if(input.isKeyPressed(GLFW_KEY_W)) 
    {
        gameObject.getTransform().translate(forward_ * movementSpeed_ * deltaTime);
        std::cout << "Moving forward";
    }

    if(input.isKeyPressed(GLFW_KEY_S)) 
    {
        gameObject.getTransform().translate(-forward_ * movementSpeed_ * deltaTime);
        std::cout << "Moving down";
    }

    if(input.isKeyPressed(GLFW_KEY_A)) 
    {
        gameObject.getTransform().translate(-right_ * movementSpeed_ * deltaTime);
        std::cout << "Moving left";
    }

      if(input.isKeyPressed(GLFW_KEY_D)) 
    {
        gameObject.getTransform().translate(right_ * movementSpeed_ * deltaTime);
        std::cout << "Moving right";
    }

}