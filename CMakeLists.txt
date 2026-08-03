cmake_minimum_required(VERSION 3.20)

project(LittleRenderer
    VERSION 0.1.0
    LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

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

# GLAD is generated C source code.
add_library(glad STATIC
    external/glad/src/gl.c
)

target_include_directories(glad
    PUBLIC
        external/glad/include
)

add_executable(LittleRenderer
    src/main.cpp
    
    src/graphics/Shader.cpp
    src/graphics/ShaderProgram.cpp

    src/Mesh.cpp
    src/VertexBuffer.cpp
    src/VertexArray.cpp
    src/PrimitiveMesh.cpp
    src/IndexBuffer.cpp

    src/GameObject.cpp
    src/Transform.cpp
    src/Renderer.cpp

    src/Camera.cpp
    src/Scene.cpp
    src/RenderComponent.h


    src/math/Mat4.cpp
    src/math/Vec3.cpp
    src/math/MathUtils.h

)

add_custom_command(
    TARGET LittleRenderer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:LittleRenderer>/assets"
)

target_link_libraries(LittleRenderer
    PRIVATE
        glad
        glfw
)

target_include_directories(LittleRenderer PRIVATE src)

# Helpful warnings.
if(MSVC)
    target_compile_options(LittleRenderer PRIVATE /W4 /permissive-)
else()
    target_compile_options(LittleRenderer PRIVATE -Wall -Wextra -Wpedantic)
endif()