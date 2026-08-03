#pragma once

#include <vector>

#include "GameObject.h"
#include "SceneTypes.h"

class Scene
{
private:
    std::vector<GameObject> gameObjects_;

public:
    GameObjectID createGameObject();

    GameObject& getGameObject(GameObjectID id);
    const GameObject& getGameObject(GameObjectID id) const;

    const std::vector<GameObject>& getGameObjects() const;
    std::vector<GameObject>& getGameObjects();
};