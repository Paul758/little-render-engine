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

    src/graphics/BasicMaterial.cpp
    src/graphics/GrassMaterial.cpp
    src/graphics/TerrainMaterial.cpp

    src/Mesh.cpp
    src/VertexBuffer.cpp
    src/VertexArray.cpp
    src/PrimitiveMesh.cpp
    src/IndexBuffer.cpp

    src/GameObject.cpp
    src/Transform.cpp
    src/Renderer.cpp

    src/Camera.cpp
    src/FreeFlyCameraController.cpp
    src/CylinderCameraController.cpp
    src/OrbitCameraController.cpp
    src/ProjectionMode.h
    src/Scene.cpp


    src/PixelFramebuffer.cpp
    src/PixelRenderer.cpp

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

    src/ecs/ComponentPool.h
    src/ecs/ComponentRegistry.h
    src/ecs/EntityManager.cpp
    src/ecs/World.cpp

    src/systems/MovementSystem.cpp

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

    tests/ComponentPoolTest.cpp
    tests/ComponentRegistryTest.cpp
    tests/EntityManagerTest.cpp
    tests/WorldTest.cpp
    tests/MovementSystemTest.cpp
)

target_link_libraries(
    LittleRendererTests

    PRIVATE
        LittleRendererCore
        Catch2::Catch2WithMain
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