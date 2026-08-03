#include "Scene.h"

GameObjectID Scene::createGameObject()
{
    gameObjects_.emplace_back();
    return gameObjects_.size() - 1;

}

GameObject& Scene::getGameObject(GameObjectID id)
{
    return gameObjects_.at(id);
}

const GameObject& Scene::getGameObject(GameObjectID id) const
{
    return gameObjects_.at(id);
}

const std::vector<GameObject>& Scene::getGameObjects() const
{
    return gameObjects_;
}

std::vector<GameObject>& Scene::getGameObjects()
{
    return gameObjects_;
}