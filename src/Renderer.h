#pragma once

#include <vector>

#include "GameObject.h"
#include "Camera.h"
#include "Scene.h"

class Renderer
{
private:
    std::vector<GameObject> gameObjects;
    Camera* camera_ = nullptr;
public:
    void render(Scene& scene, Camera& camera) const;
    GameObject& addObject(GameObject&& gameObject);
};