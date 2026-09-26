cmake_minimum_required(VERSION 3.20)

project(LittleRenderer
    VERSION 0.1.0
    LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# ==================
# Dependencies
# ==================

include(FetchContent)

#--------------------
# GLFW
#--------------------

# Prevent GLFW from building things we do not need.
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)

FetchContent_MakeAvailable(glfw)

#--------------------
# Catch2
#--------------------
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.8.1
)

FetchContent_MakeAvailable(Catch2)

enable_testing()

#--------------------
# Glad
#--------------------
add_library(glad STATIC
    external/glad/src/gl.c
)

target_include_directories(glad
    PUBLIC
        external/glad/include
)

#--------------------
# Engine library
#--------------------
add_library(LittleRendererEngine STATIC 

    # Editor
    engine/editor/Editor.cpp

    # Graphics
    engine/graphics/Shader.cpp
    engine/graphics/ShaderProgram.cpp
    engine/graphics/Texture2D.cpp
    engine/graphics/Framebuffer.cpp
    engine/graphics/IndexBuffer.cpp
    engine/graphics/Mesh.cpp
    engine/graphics/Model.cpp
    engine/graphics/VertexBuffer.cpp
    engine/graphics/VertexArray.cpp
    engine/graphics/PrimitiveMesh.cpp

    engine/graphics/materials/BasicMaterial.cpp

    engine/graphics/rendering/Renderer.cpp
    engine/graphics/rendering/ScreenRenderer.cpp

    # Math
    engine/math/Quaternion.cpp
    engine/math/Mat4.cpp
    engine/math/Vec3.cpp
    engine/math/Vec2.cpp
    engine/math/Vec4.cpp

    # Core
    engine/core/input/Input.cpp
    engine/core/time/GameTime.cpp

    # Asset Manager
    engine/assets/AssetManager.cpp
    engine/assets/TextureLoader.cpp
    engine/assets/ModelLoader.cpp
    engine/assets/ModelInstantiator.cpp

    # Components
    engine/components/TransformComponent.cpp
    engine/components/TransformUtils.cpp

    # ECS
    engine/core/ecs/EntityManager.cpp
    engine/core/ecs/World.cpp

    # Systems
    engine/systems/RenderSystem.cpp
    engine/systems/BehaviourTreeSystem.cpp
    engine/systems/CameraSystem.cpp
    engine/systems/EditorCameraUpdateSystem.cpp
    engine/systems/EditorCameraInputSystem.cpp
    engine/systems/LightingSystem.cpp
    engine/systems/HierarchySystem.cpp
    engine/systems/TransformSystem.cpp

    # Behaviour
    engine/behaviour/BehaviourTree.cpp
    engine/behaviour/composites/Sequence.cpp
    engine/behaviour/composites/Selector.cpp
    engine/behaviour/composites/EvaluateAll.cpp

    # TinyGltf
    engine/assets/TinyGltfImplementation.cpp
)

target_include_directories(
    LittleRendererEngine

    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}

    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/external/stb
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/external/tinygltf
)

target_link_libraries(LittleRendererEngine
    PUBLIC
        glad
        glfw
)

target_compile_definitions(LittleRendererEngine
    PUBLIC
        GLFW_INCLUDE_NONE
)

#--------------------
# Game Library
#--------------------
add_library(LittleRendererGame STATIC
    
    game/Game.cpp

    # Behaviour
    game/behaviour/player/PlayerMovementBTreeBuilder.cpp

    # Graphics
    game/graphics/materials/GrassMaterial.cpp
    game/graphics/materials/TerrainMaterial.cpp

    # Grass
    game/grass/Terrain.cpp

    # Rendering
    game/rendering/PixelFramebuffer.cpp
    game/rendering/PixelRenderer.cpp

    # Game Systems
    game/systems/camera/FixedOrbitCameraSystem.cpp
    game/systems/ActionResolver.cpp
    game/systems/CommandBuffer.cpp
    game/systems/IntegrationSystem.cpp
    game/systems/IntentResetSystem.cpp
    game/systems/LocomotionSystem.cpp
    game/systems/PlayerInputSystem.cpp
)


target_include_directories(LittleRendererGame

    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(LittleRendererGame

    PUBLIC
        LittleRendererEngine
)

target_compile_definitions(LittleRendererGame
    PUBLIC
        GLFW_INCLUDE_NONE
)

#--------------------
# Main executable
#--------------------
add_executable(
    LittleRenderer

    app/main.cpp
    app/Application.cpp
)

target_link_libraries(
    LittleRenderer
    PRIVATE
    LittleRendererEngine
    LittleRendererGame
)

#--------------------
# Tests
#--------------------
add_executable(
    LittleRendererTests

    tests/math/Mat4Test.cpp
    tests/math/QuaternionTest.cpp

    tests/ComponentPoolTest.cpp
    tests/ComponentRegistryTest.cpp
    tests/EntityManagerTest.cpp
    tests/WorldTest.cpp

    tests/IntegrationSystemTest.cpp
    tests/InputSystemTest.cpp
    tests/IntentResetSystemTest.cpp
    tests/ActionResolverTest.cpp
    tests/LocomotionSystemTest.cpp
    tests/CameraSystemTest.cpp
    tests/engine/systems/HierarchySystemTest.cpp
    tests/engine/systems/TransformSystemTest.cpp

    tests/behaviour/BehaviourTreeRegistryTest.cpp
    tests/behaviour/BehaviourTreeSystemTest.cpp
    tests/behaviour/composites/SequenceTest.cpp
    tests/behaviour/composites/SelectorTest.cpp
    tests/behaviour/composites/EvaluateAllTest.cpp
    tests/behaviour/player/PlayerMovementNodesTest.cpp

    tests/engine/assets/AssetManagerTest.cpp
    tests/engine/assets/ModelLoaderTest.cpp
    tests/engine/assets/ModelInstantiatorTest.cpp

    tests/helpers/OpenGLTestContext.cpp


    tests/fakes/FakeInput.h
)

target_link_libraries(
    LittleRendererTests

    PRIVATE
        LittleRendererEngine
        LittleRendererGame
        Catch2::Catch2WithMain
)

target_include_directories(LittleRendererTests
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/tests
)

target_compile_definitions(
    LittleRendererTests
    PRIVATE
    TEST_ASSET_DIR="${CMAKE_SOURCE_DIR}/assets"    
)

include(CTest)
include(Catch)

catch_discover_tests(
    LittleRendererTests
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

#--------------------
# Assets
#--------------------
add_custom_command(
    TARGET LittleRenderer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:LittleRenderer>/assets"
)


#--------------------
# Warnings
#--------------------
if(MSVC)
    target_compile_options(LittleRenderer PRIVATE /W4 /permissive-)
else()
    target_compile_options(LittleRenderer PRIVATE -Wall -Wextra -Wpedantic)
endif()