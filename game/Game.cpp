#include "game/Game.h"

#include "engine/components/TransformComponent.h"
#include "engine/components/RenderComponent.h"
#include "engine/components/BehaviourTreeComponent.h"
#include "engine/components/NameComponent.h"

#include "engine/components/camera/CameraComponent.h"
#include "engine/components/camera/WorldCameraComponent.h"
#include "engine/components/lighting/DirectionalLightComponent.h"

#include "engine/graphics/Mesh.h"
#include "engine/graphics/PrimitiveMesh.h"
#include "engine/graphics/ShaderProgram.h"
#include "engine/graphics/materials/BasicMaterial.h"
#include "engine/graphics/Model.h"

#include "game/components/PlayerInputComponent.h"
#include "game/components/LocomotionIntentComponent.h"
#include "game/components/VelocityComponent.h"
#include "game/behaviour/player/PlayerMovementBTreeBuilder.h"
#include "game/components/camera/FixedOrbitCameraComponent.h"
#include "game/components/camera/OrbitPose.h"
#include "game/components/HealthComponent.h"

#include "engine/math/Vec3.h"

#include "engine/assets/AssetManager.h"

#include <vector>

Game::Game(AssetManager& assets)
    : assets_(assets),
      playerInputSystem_(world_.components()),
      behaviourTreeSystem_(world_.components(), behaviourTreeRegistry_),
      intentResetSystem_(world_.components()),
      locomotionSystem_(world_.components()),
      actionResolver_(world_.components()),
      integrationSystem_(world_.components()),
      fixedOrbitCameraSystem_(world_.components()),
      hierarchySystem_(world_.components()),
      transformSystem_(world_.components(), hierarchySystem_),
      modelInstantiator_(world_, hierarchySystem_)
{
}

void Game::initialize()
{
    createPlayer();
    createScene();
    createCamera();
    createLight();
    
    fixedOrbitCameraSystem_.initialize(gameCamera_);
}

void Game::createScene()
{
    //Resources
    ShaderProgram& standardShader = assets_.loadShader("shaders/standard.vert", "shaders/standard.frag");
    basicMaterial_ = std::make_unique<BasicMaterial>(standardShader);

    basicMaterial_->albedoColor = Vec3{1.0f, 1.0f, 1.0f};
    Texture2D& brick = assets_.loadTexture("textures/brick.png");
    basicMaterial_->albedoTexture = &brick;

    Model& cubeModel = assets_.loadModel("models/cube.glb");
    Entity cubeA = modelInstantiator_.instantiate(cubeModel);

    Model& robotModel = assets_.loadModel("models/robot.glb");
    Entity robot = modelInstantiator_.instantiate(robotModel);

    TransformComponent robotTransform;
    robotTransform.position = Vec3{5.0f, 0.0f, 5.0f};
    robotTransform.rotation = Quaternion::fromAxisAngle(Vec3{0.0f, 1.0f, 0.0f}, 45.0f);
    world_.components().add(robot, robotTransform);
    world_.components().add(robot, NameComponent{"Robot"});

    Model& truckModel = assets_.loadModel("models/CesiumMilkTruck.glb");
    Entity truck = modelInstantiator_.instantiate(truckModel);

    TransformComponent truckTransform;
    truckTransform.position = Vec3{-10.0f, 0.0f, -10.0f};
    truckTransform.rotation = Quaternion::fromAxisAngle(Vec3{0.0f, 1.0f, 0.0f}, 45.0f);
    world_.components().add(truck, truckTransform);
    world_.components().add(truck, NameComponent{"Milk Truck"});
    

    TransformComponent cubeATransform;
    cubeATransform.position = Vec3{-2.0f, 0.0f, -2.0f};
    world_.components().add(cubeA, cubeATransform);
    world_.components().add(cubeA, NameComponent{"Cube"});

    // Sphere
    MeshData sphereData = PrimitiveMesh::sphere(8, 4);
    sphereMesh_ = std::make_unique<Mesh>(sphereData);
    
    Entity sphereA = world_.createEntity();
    TransformComponent transformSphere;
    transformSphere.position = Vec3{5.0f, 0.0f, 1.0f};

    world_.components().add(sphereA, transformSphere);
    world_.components().add(sphereA, RenderComponent{sphereMesh_.get(), basicMaterial_.get()});
    world_.components().add(sphereA, NameComponent{"Sphere"});
}

void Game::createPlayer()
{
    player_ = world_.createEntity();

    Model& cubeModel = assets_.loadModel("models/cube.glb");
    if (cubeModel.getMeshCount() == 0)
    {
        throw std::runtime_error("cube.glb contains no meshes");
    }

    ModelMesh& cubeModelMesh = cubeModel.getMesh(0);
    ModelPrimitive& cubePrimitive = cubeModelMesh.primitives.at(0);

    if (cubePrimitive.materialIndex >= 0)
    {
        Mesh& cubeMesh = *cubePrimitive.mesh;
        Material& cubeMaterial = cubeModel.getMaterial(static_cast<std::size_t>(cubePrimitive.materialIndex));
        world_.components().add(player_, RenderComponent{&cubeMesh, &cubeMaterial});
    }

    TransformComponent playerTransform;
    playerTransform.position = Vec3{1.0f, 0.0f, -1.0f};

    world_.components().add(player_, playerTransform);
    world_.components().add(player_, PlayerInputComponent{});
    world_.components().add(player_, LocomotionIntentComponent{});
    world_.components().add(player_, VelocityComponent{});

    PlayerMovementBTreeBuilder builder;

    auto tree = builder.build();

    BehaviourTreeId treeId = behaviourTreeRegistry_.add(std::move(tree));

    world_.components().add(player_, BehaviourTreeComponent{treeId});
    world_.components().add(player_, NameComponent{"Player"});

    world_.components().add(player_, HealthComponent{.currentHealth = 75.0f, .maxHealth = 100.0f});
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

    CameraComponent camera{ProjectionType::Orthographic, 20.0f};

    world_.components().add(gameCamera_, TransformComponent{});
    world_.components().add(gameCamera_, camera);
    world_.components().add(gameCamera_, WorldCameraComponent{});
    world_.components().add(gameCamera_, orbitCamera);

    world_.components().add(gameCamera_, NameComponent{"Game Camera"});

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
    world_.components().add(sun, NameComponent{"Sun light"});
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

    transformSystem_.update();
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

HierarchySystem& Game::getHiearchySystem()
{
    return hierarchySystem_;
}

Game::~Game() = default;