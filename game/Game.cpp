#include "game/Game.h"

#include "engine/components/TransformComponent.h"
#include "engine/components/RenderComponent.h"
#include "engine/components/BehaviourTreeComponent.h"

#include "engine/components/camera/CameraComponent.h"
#include "engine/components/camera/WorldCameraComponent.h"
#include "engine/components/lighting/DirectionalLightComponent.h"

#include "engine/graphics/Mesh.h"
#include "engine/graphics/PrimitiveMesh.h"
#include "engine/graphics/ShaderProgram.h"
#include "engine/graphics/materials/BasicMaterial.h"

#include "game/components/PlayerInputComponent.h"
#include "game/components/LocomotionIntentComponent.h"
#include "game/components/VelocityComponent.h"
#include "game/behaviour/player/PlayerMovementBTreeBuilder.h"
#include "game/components/camera/FixedOrbitCameraComponent.h"
#include "game/components/camera/OrbitPose.h"

#include "engine/math/Vec3.h"

#include <vector>

Game::Game()
    : playerInputSystem_(world_.components()),
      behaviourTreeSystem_(world_.components(), behaviourTreeRegistry_),
      intentResetSystem_(world_.components()),
      locomotionSystem_(world_.components()),
      actionResolver_(world_.components()),
      integrationSystem_(world_.components()),
      fixedOrbitCameraSystem_(world_.components())
{
}

void Game::initialize()
{
    createScene();
    createPlayer();
    createCamera();
    createLight();
    
    fixedOrbitCameraSystem_.initialize(gameCamera_);
}

void Game::createScene()
{
    //Resources
    standardShader_ = std::make_unique<ShaderProgram>("assets/shaders/standard.vert", "assets/shaders/standard.frag");
    basicMaterial_ = std::make_unique<BasicMaterial>(*standardShader_);

    basicMaterial_->albedoColor = Vec3{0.7f, 0.7f, 0.2f};

    MeshData cubeData = PrimitiveMesh::cube();

    cubeMesh_ = std::make_unique<Mesh>(cubeData);

    Entity cubeA = world_.createEntity();
    Entity cubeB = world_.createEntity();
    Entity cubeC = world_.createEntity();

    world_.components().add(cubeA, TransformComponent{});
    world_.components().add(cubeB, TransformComponent{});
    world_.components().add(cubeC, TransformComponent{});

    world_.components().add(cubeA, RenderComponent{cubeMesh_.get(), basicMaterial_.get()});
    world_.components().add(cubeB, RenderComponent{cubeMesh_.get(), basicMaterial_.get()});
    world_.components().add(cubeC, RenderComponent{cubeMesh_.get(), basicMaterial_.get()});
}

void Game::createPlayer()
{
    player_ = world_.createEntity();

    world_.components().add(player_, TransformComponent{});
    world_.components().add(player_, RenderComponent{cubeMesh_.get(), basicMaterial_.get()});
    world_.components().add(player_, PlayerInputComponent{});
    world_.components().add(player_, LocomotionIntentComponent{});
    world_.components().add(player_, VelocityComponent{});

    PlayerMovementBTreeBuilder builder;

    auto tree = builder.build();

    BehaviourTreeId treeId = behaviourTreeRegistry_.add(std::move(tree));

    world_.components().add(player_, BehaviourTreeComponent{treeId});
}

void Game::createCamera()
{
    gameCamera_ = world_.createEntity();

    std::vector<OrbitPose> orbitPoses{
        {45.0f, 30.0f},
        {135.0f, 30.0f},
        {225.0f, 30.0f},
        {315.0f, 30.0f}
    };

    TransformComponent* playerTransform = world_.components().get<TransformComponent>(player_);

    if (playerTransform == nullptr)
    {
        std::cout << "There is not player Transform";
    }

    FixedOrbitCameraComponent orbitCamera{
        playerTransform->position,
        orbitPoses,
        0,
        8.0f,
        10.0f
    };

    CameraComponent camera{ProjectionType::Orthographic, 10.0f};

    world_.components().add(gameCamera_, TransformComponent{});
    world_.components().add(gameCamera_, camera);
    world_.components().add(gameCamera_, WorldCameraComponent{});
    world_.components().add(gameCamera_, orbitCamera);

}

void Game::createLight()
{
    Entity sun = world_.createEntity();
    DirectionalLightComponent light;
    light.color = Vec3{1.0f, 0.9f, 0.9f};
    light.intensity = 1.0f;

    TransformComponent transform;
    transform.position = Vec3{3.0f, 5.0f, 3.0f};
    transform.rotation = Quaternion::lookRotation(Vec3{0.0f, 0.0f, 0.0f} - transform.position, Vec3{0.0f, 1.0f, 0.0f});

    world_.components().add(sun, transform);
    world_.components().add(sun, light);
}

void Game::update(const Input& input, float deltaTime)
{
    commandBuffer_.clear();
    intentResetSystem_.update();
    playerInputSystem_.update(input);
    behaviourTreeSystem_.update();
    actionResolver_.update(commandBuffer_);
    locomotionSystem_.update(commandBuffer_);
    integrationSystem_.update(deltaTime);

    fixedOrbitCameraSystem_.update(input, gameCamera_, deltaTime);

}

Entity Game::getCamera() const
{
    return gameCamera_;
}

World& Game::getWorld()
{
    return world_;
}

const World& Game::getWorld() const
{
    return world_;
}

Game::~Game() = default;