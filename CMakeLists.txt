cmake_minimum_required(VERSION 3.20)

project(CHOTANENGINE VERSION 0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# ===== GLAD =====
add_library(glad external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)

# ===== ENGINE =====
add_executable(engine
    engine/main.cpp

    engine/core/Input.cpp
    engine/math/Camera.cpp

    # renderer (ya los tienes)
    engine/renderer/Shader.cpp
    engine/renderer/Texture.cpp
    engine/renderer/Mesh.cpp
    engine/renderer/MTLLoader.cpp
    engine/renderer/OBJLoader.cpp
    engine/renderer/Model.cpp
    engine/renderer/ModelRenderer.cpp

    # systems
    engine/systems/RenderSystem.cpp
    engine/systems/PlayerControllerSystem.cpp

    # scene
    engine/scene/SceneLoader.cpp
)

target_include_directories(engine PRIVATE
    engine
    external/glad/include
    external/glfw/include
    external/glm
    external/stb
)

target_link_directories(engine PRIVATE external/glfw/lib-vc2022)
target_link_libraries(engine PRIVATE glad glfw3)

if (WIN32)
    target_link_libraries(engine PRIVATE opengl32)
endif()

# ===== COPY SHADERS & ASSETS =====
add_custom_target(copy_runtime_files ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/shaders
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets
)

add_dependencies(engine copy_runtime_files)
