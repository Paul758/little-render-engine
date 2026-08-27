#include <GLFW/glfw3.h>

#include "GameTime.h"

GameTime::GameTime()
{
    lastFrameTime_ = static_cast<float>(glfwGetTime());
}

void GameTime::update()
{
    currentTime_ = static_cast<float>(glfwGetTime());
    deltaTime_ = currentTime_ - lastFrameTime_;
    lastFrameTime_ = currentTime_;
}

float GameTime::deltaTime() const
{
    return deltaTime_;
}