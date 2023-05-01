cmake_minimum_required(VERSION 3.24)
project(ShitCraft VERSION 0.0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)

set(BUILD_SHARED_LIBS OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static -static-libgcc -static-libstdc++")

if (${CMAKE_BUILD_TYPE} STREQUAL Debug)
    set(DEBUG ON)
else ()
    set(DEBUG OFF)
endif ()

# Check python installation with required modules

find_package(PythonInterp)

if (NOT DEFINED PYTHONINTERP_FOUND)
    message(FATAL_ERROR "Python interpreter not found.")
endif ()

execute_process(
        COMMAND ${PYTHON_EXECUTABLE} -c "import jinja2"
        RESULT_VARIABLE EXIT_CODE
        OUTPUT_QUIET
        ERROR_QUIET
)

if (NOT (${EXIT_CODE} EQUAL "0"))
    message(FATAL_ERROR "jinja2 module for Python not found.")
endif ()

# Resolve dependencies using FetchContent

include(FetchContent)

FetchContent_Declare(glad URL https://github.com/Dav1dde/glad/archive/refs/tags/v2.0.4.tar.gz)
FetchContent_Declare(cmrc URL https://github.com/vector-of-bool/cmrc/archive/refs/tags/2.0.1.tar.gz)
FetchContent_Declare(SDL2 URL https://github.com/libsdl-org/SDL/archive/refs/tags/release-2.26.5.tar.gz)
FetchContent_Declare(GLM URL https://github.com/g-truc/glm/archive/refs/tags/0.9.9.8.tar.gz)
FetchContent_Declare(spdlog URL https://github.com/gabime/spdlog/archive/refs/tags/v1.11.0.tar.gz)
FetchContent_Declare(nlohmann_json URL https://github.com/nlohmann/json/archive/refs/tags/v3.11.2.tar.gz)
FetchContent_Declare(nlohmann_json_schema_validator URL https://github.com/pboettch/json-schema-validator/archive/refs/tags/2.2.0.zip)

set(SDL2_DISABLE_INSTALL ON CACHE BOOL "" FORCE)
set(JSON_VALIDATOR_INSTALL OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(SDL2 GLM spdlog glad cmrc nlohmann_json nlohmann_json_schema_validator)

add_subdirectory(${glad_SOURCE_DIR}/cmake ${glad_BINARY_DIR})

if (DEBUG)
    glad_add_library(glad_gl_core_33 REPRODUCIBLE DEBUG API gl:core=3.3)
else ()
    glad_add_library(glad_gl_core_33 REPRODUCIBLE API gl:core=3.3)
endif ()

if (NOT EXISTS ${CMAKE_BINARY_DIR}/stb/stb_image.h)
    file(DOWNLOAD https://raw.githubusercontent.com/nothings/stb/master/stb_image.h ${CMAKE_BINARY_DIR}/stb/stb_image.h)
endif()

if (NOT EXISTS ${CMAKE_BINARY_DIR}/stb/stb_image_resize.h)
    file(DOWNLOAD https://raw.githubusercontent.com/nothings/stb/master/stb_image_resize.h ${CMAKE_BINARY_DIR}/stb/stb_image_resize.h)
endif()

# Build project

set(EXECUTABLE_TARGET_NAME ShitCraft)

set(SRC
        ${CMAKE_BINARY_DIR}/src/config.h
        src/external/stb.cpp
        src/main.cpp
        src/Game.cpp
        src/Game.h
        src/config.h.in
        src/renderer/Shader.cpp
        src/renderer/Shader.h
        src/types.h
        src/core/Drawable.h
        src/core/EventConsumer.h
        src/core/EventConsumer.h
        src/core/Node.cpp
        src/core/Node.h
        src/core/NodeHost.h
        src/core/Node.h
        src/core/NodeHost.cpp
        src/core/Node.cpp
        src/renderer/Transform.h
        src/SimpleCube.cpp
        src/SimpleCube.h
        src/scene/SimpleCubeScene.h
        src/camera/Camera.h
        src/camera/CameraControls.h
        src/camera/CameraControls.cpp
        src/core/UpdateDelegate.h
        src/core/INode.h
        src/renderer/Texture.h
        src/renderer/Texture.cpp
        src/utils.cpp
        src/utils.h
        src/Context.h
        src/Context.cpp
        src/renderer/Vertex.h
        src/arch.h
        src/asset/Provider.h
        src/asset/Manager.cpp
        src/asset/Manager.h
        src/asset/Asset.h
        src/renderer/ShaderType.h
        src/asset/Cache.h
        src/asset/FileReader.h
        src/asset/Index.h
        src/asset/Parser.cpp src/asset/Parser.h src/asset/LocalFileReader.cpp src/asset/LocalFileReader.h src/asset/Image.cpp src/asset/Image.h src/PixelFormat.h src/world/Chunk.cpp src/world/Chunk.h src/renderer/BlocksRenderer.cpp src/renderer/BlocksRenderer.h src/renderer/ChunkMesh.cpp src/renderer/ChunkMesh.h src/world/ChunkCache.cpp src/world/ChunkCache.h src/scene/WorldScene.cpp src/scene/WorldScene.h src/world/BlockCache.cpp src/world/BlockCache.h src/world/Block.h src/world/Block.cpp src/renderer/ChunkMeshBuilder.cpp src/renderer/ChunkMeshBuilder.h)

set(LIBS SDL2::SDL2main SDL2::SDL2-static glad_gl_core_33 glm::glm spdlog::spdlog nlohmann_json nlohmann_json_schema_validator)

if (DEBUG)
    set(ASSETS_DIR ${PROJECT_SOURCE_DIR}/assets/)
else ()
    set(ASSETS_PREFIX assets/)
    cmrc_add_resource_library(assets assets)
    set(LIBS assets ${LIBS})
endif ()

configure_file(src/config.h.in ${CMAKE_BINARY_DIR}/src/config.h)

add_executable(${EXECUTABLE_TARGET_NAME} ${SRC})

target_link_libraries(${EXECUTABLE_TARGET_NAME} PRIVATE ${LIBS})
target_include_directories(${EXECUTABLE_TARGET_NAME} PRIVATE ${CMAKE_BINARY_DIR}/src ${CMAKE_BINARY_DIR}/stb ${CMAKE_BINARY_DIR}/json src)