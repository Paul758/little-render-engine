#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <cstdlib>
#include <iostream>
#include <filesystem>
#include <vector>

#include "engine/graphics/ShaderProgram.h"
#include "engine/graphics/Mesh.h"
#include "engine/graphics/PrimitiveMesh.h"
#include "engine/math/MathUtils.h"
#include "game/rendering/PixelFramebuffer.h"
#include "game/grass/Terrain.h"
#include "engine/graphics/Texture2D.h"
#include "engine/graphics/materials/BasicMaterial.h"
#include "game/graphics/materials/GrassMaterial.h"
#include "game/graphics/materials/TerrainMaterial.h"
#include "engine/graphics/rendering/RenderViewport.h"
#include "game/rendering/PixelRenderer.h"
#include "engine/core/time/GameTime.h"
#include "engine/graphics/rendering/Renderer.h"

#include "game/components/PlayerInputComponent.h"
#include "engine/components/TransformComponent.h"
#include "engine/components/RenderComponent.h"
#include "game/components/LocomotionIntentComponent.h"
#include "engine/components/BehaviourTreeComponent.h"
#include "game/components/VelocityComponent.h"
#include "engine/components/camera/CameraComponent.h"
#include "engine/components/camera/FreeFlyCameraComponent.h"
#include "engine/components/camera/FreeHandCameraComponent.h"
#include "engine/components/camera/WorldCameraComponent.h"
#include "game/components/camera/FixedOrbitCameraComponent.h"
#include "game/components/camera/OrbitPose.h"
#include "engine/components/lighting/DirectionalLightComponent.h"


#include "engine/systems/RenderSystem.h"
#include "game/systems/PlayerInputSystem.h"
#include "game/systems/IntentResetSystem.h"
#include "game/systems/LocomotionSystem.h"
#include "game/systems/ActionResolver.h"
#include "game/systems/CommandBuffer.h"
#include "game/systems/IntegrationSystem.h"
#include "engine/systems/BehaviourTreeSystem.h"
#include "engine/systems/CameraSystem.h"
#include "engine/systems/EditorCameraUpdateSystem.h"
#include "game/systems/camera/FixedOrbitCameraSystem.h"
#include "engine/systems/LightingSystem.h"

#include "game/behaviour/player/PlayerMovementBTreeBuilder.h"
#include "engine/behaviour/BehaviourContext.h"
#include "engine/behaviour/BehaviourTreeRegistry.h"

#include "engine/core/ecs/World.h"
#include "engine/graphics/rendering/ScreenRenderer.h"

#include "Application.h"

int main()
{
    Application application;

    if(!application.initialize())
    {
        return EXIT_FAILURE;
    }

    application.run();

    return EXIT_SUCCESS;
}