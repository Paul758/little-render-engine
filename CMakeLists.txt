cmake_minimum_required(VERSION 3.20)

project(LittleRenderer
    VERSION 0.1.0
    LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# Prevent GLFW from building things we do not need.
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

include(FetchContent)

#--------------------
# GLFW
#--------------------
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
add_library(LittleRendererCore  
    src/graphics/Shader.cpp
    src/graphics/ShaderProgram.cpp
    src/graphics/Texture2D.cpp
    src/graphics/PixelSettings.h
    src/graphics/RenderView.h
    src/graphics/RenderViewport.h

    src/graphics/materials/BasicMaterial.cpp
    src/graphics/materials/GrassMaterial.cpp
    src/graphics/materials/TerrainMaterial.cpp


    src/Mesh.cpp
    src/VertexBuffer.cpp
    src/VertexArray.cpp
    src/PrimitiveMesh.cpp
    src/IndexBuffer.cpp

    src/GameObject.cpp
    src/Transform.cpp

    src/Camera.cpp
    src/FreeFlyCameraController.cpp
    src/CylinderCameraController.cpp
    src/OrbitCameraController.cpp
    src/ProjectionMode.h
    src/Scene.cpp


    src/PixelFramebuffer.cpp
    src/PixelRenderer.cpp

    src/math/Quaternion.cpp
    src/math/Mat4.cpp
    src/math/Vec3.cpp
    src/math/Vec2.cpp
    src/math/MathUtils.h

    src/PlayerController.cpp
    src/Input.cpp

    src/grass/GrassType.h
    src/grass/Terrain.cpp
    src/grass/TerrainCell.h

    src/components/TransformComponent.cpp
    src/components/RenderComponent.h
    src/components/PlayerInputComponent.h
    src/components/camera/CameraComponent.h
    src/components/camera/FreeFlyCameraComponent.h
    src/components/camera/FreeHandCameraComponent.h
    src/components/camera/WorldCameraComponent.h
    src/components/TransformUtils.cpp
    src/components/camera/EditorCameraInputComponent.h

    src/ecs/ComponentPool.h
    src/ecs/ComponentRegistry.h
    src/ecs/EntityManager.cpp
    src/ecs/World.cpp

    src/systems/IntegrationSystem.cpp
    src/systems/RenderSystem.cpp
    src/systems/InputSystem.cpp
    src/systems/BehaviourTreeSystem.cpp
    src/systems/IntentResetSystem.cpp
    src/systems/ActionResolver.cpp
    src/systems/LocomotionSystem.cpp
    src/systems/CommandBuffer.cpp
    src/systems/CameraSystem.cpp
    src/systems/EditorCameraUpdateSystem.cpp
    src/systems/EditorSystem.cpp
    src/systems/EditorCameraInputSystem.cpp

    src/behaviour/BehaviourTreeRegistry.h
    src/behaviour/BehaviourTree.cpp
    src/behaviour/NodeState.h
    src/behaviour/composites/Sequence.cpp
    src/behaviour/composites/Selector.cpp
    src/behaviour/composites/EvaluateAll.cpp
    src/behaviour/player/PlayerMovementBTreeBuilder.cpp

    src/GameTime.cpp
)

target_include_directories(
    LittleRendererCore

    PUBLIC
    src

    PRIVATE
    external/stb
)

target_link_libraries(LittleRendererCore
    PUBLIC
        glad
        glfw
)

#--------------------
# Main executable
#--------------------
add_executable(LittleRenderer
    src/main.cpp
)

target_link_libraries(
    LittleRenderer
    PRIVATE
    LittleRendererCore
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

    tests/behaviour/BehaviourTreeRegistryTest.cpp
    tests/behaviour/BehaviourTreeSystemTest.cpp
    tests/behaviour/composites/SequenceTest.cpp
    tests/behaviour/composites/SelectorTest.cpp
    tests/behaviour/composites/EvaluateAllTest.cpp
    tests/behaviour/player/PlayerMovementNodesTest.cpp



    tests/fakes/FakeInput.h
)

target_link_libraries(
    LittleRendererTests

    PRIVATE
        LittleRendererCore
        Catch2::Catch2WithMain
)

target_include_directories(LittleRendererTests
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/tests
)

include(CTest)
include(Catch)

catch_discover_tests(
    LittleRendererTests
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