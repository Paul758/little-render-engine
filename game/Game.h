#pragma once

#include "engine/core/ecs/World.h"
#include "engine/behaviour/BehaviourTreeRegistry.h"
#include "engine/systems/BehaviourTreeSystem.h"

#include "game/systems/PlayerInputSystem.h"
#include "game/systems/IntentResetSystem.h"
#include "game/systems/LocomotionSystem.h"
#include "game/systems/ActionResolver.h"
#include "game/systems/CommandBuffer.h"
#include "game/systems/IntegrationSystem.h"
#include "game/systems/camera/FixedOrbitCameraSystem.h"

class ShaderProgram;
class BasicMaterial;
class Mesh;

class Input;

class Game
{
public:
    Game();
    ~Game();

    void initialize();
    void update(const Input& input, float deltaTime);    

    World& getWorld();
    const World& getWorld() const;

    Entity getCamera() const;

private:
    void createScene();
    void createPlayer();
    void createCamera();
    void createLight();

private:
    Entity player_;
    Entity gameCamera_;
    //Graphics resources
    std::unique_ptr<ShaderProgram> standardShader_;
    std::unique_ptr<BasicMaterial> basicMaterial_;
    std::unique_ptr<Mesh> cubeMesh_;

    //ECS
    World world_;

    FixedOrbitCameraSystem fixedOrbitCameraSystem_;
    //Behaviour
    BehaviourTreeRegistry behaviourTreeRegistry_;

    //Gameplay systems
    PlayerInputSystem playerInputSystem_;
    BehaviourTreeSystem behaviourTreeSystem_;
    IntentResetSystem intentResetSystem_;
    LocomotionSystem locomotionSystem_;
    ActionResolver actionResolver_;
    IntegrationSystem integrationSystem_;
    CommandBuffer commandBuffer_;
};