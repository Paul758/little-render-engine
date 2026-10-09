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
# JSON
#--------------------
FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.12.0
)

FetchContent_MakeAvailable(nlohmann_json)

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
# ImGui library
#--------------------
add_library(imgui STATIC
    external/imgui/imgui.cpp
    external/imgui/imgui_demo.cpp
    external/imgui/imgui_draw.cpp
    external/imgui/imgui_tables.cpp
    external/imgui/imgui_widgets.cpp

    external/imgui/backends/imgui_impl_glfw.cpp
    external/imgui/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui
    PUBLIC
        external/imgui
        external/imgui/backends
)

target_link_libraries(imgui
    PUBLIC
        glfw
)

#--------------------
# Engine library
#--------------------
add_library(LittleRendererEngine STATIC 

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

    # Serialization
    engine/serialization/ComponentSerializationRegistry.cpp
    engine/serialization/EngineSerialization.cpp
    engine/serialization/SerializationUtils.cpp

    # Scene
    engine/scene/SceneSerializer.cpp
    engine/scene/SceneJson.cpp
    engine/scene/SceneFile.cpp
    engine/scene/SceneService.cpp
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
        nlohmann_json::nlohmann_json
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

    # Serialization
    game/serialization/GameSerialization.cpp

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
# Editor library
#--------------------
add_library(LittleRendererEditor STATIC
    editor/Editor.cpp

    editor/ComponentEditorRegistry.cpp

    editor/panels/HierarchyPanel.cpp
    editor/panels/InspectorPanel.cpp

    editor/component-editors/TransformEditor.cpp
    editor/component-editors/CameraEditor.cpp
    editor/component-editors/NameEditor.cpp
    editor/component-editors/RenderComponentEditor.cpp


)

target_link_libraries(LittleRendererEditor
    PUBLIC
        LittleRendererEngine
    PRIVATE
        imgui
)

target_include_directories(LittleRendererEditor
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

#--------------------
# Game Editor library
#--------------------
add_library(LittleRendererGameEditor STATIC
    game-editor/GameEditor.cpp
)

target_link_libraries(LittleRendererGameEditor
    PUBLIC
        LittleRendererGame
        LittleRendererEditor
    PRIVATE
        imgui

)

target_include_directories(LittleRendererGameEditor
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
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
    LittleRendererEditor
    LittleRendererGameEditor
    imgui
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

    tests/engine/serialization/ComponentSerializationRegistryTest.cpp
    tests/engine/serialization/SerializedValueTest.cpp
    tests/engine/serialization/SerializationUtilsTest.cpp

    tests/engine/scene/SceneSerializerTest.cpp
    tests/engine/scene/SceneJsonTest.cpp
    tests/engine/scene/SceneFileTest.cpp
    tests/engine/scene/SceneServiceTest.cpp

    tests/game/GameSerializationTest.cpp

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