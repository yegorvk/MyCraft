cmake_minimum_required(VERSION 3.24)
project(ShitCraft VERSION 0.0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)

set(BUILD_SHARED_LIBS OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static -static-libgcc -static-libstdc++")

if (${CMAKE_BUILD_TYPE} STREQUAL Debug)
    set(DEBUG ON)
else ()
    set(DEBUG OFF)
endif ()

# Resolve dependencies using FetchContent

include(FetchContent)

FetchContent_Declare(cmrc URL https://github.com/vector-of-bool/cmrc/archive/refs/tags/2.0.1.tar.gz)
FetchContent_Declare(SDL2 URL https://github.com/libsdl-org/SDL/archive/refs/tags/release-2.26.5.tar.gz)
FetchContent_Declare(GLM URL https://github.com/g-truc/glm/archive/refs/tags/0.9.9.8.tar.gz)
FetchContent_Declare(spdlog URL https://github.com/gabime/spdlog/archive/refs/tags/v1.11.0.tar.gz)
FetchContent_Declare(nlohmann_json URL https://github.com/nlohmann/json/archive/refs/tags/v3.11.2.tar.gz)
FetchContent_Declare(nlohmann_json_schema_validator URL https://github.com/pboettch/json-schema-validator/archive/refs/tags/2.2.0.zip)

set(SDL2_DISABLE_INSTALL ON CACHE BOOL "" FORCE)
set(JSON_VALIDATOR_INSTALL OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(SDL2 GLM spdlog cmrc nlohmann_json nlohmann_json_schema_validator)

if (NOT EXISTS ${CMAKE_BINARY_DIR}/stb/stb_image.h)
    file(DOWNLOAD https://raw.githubusercontent.com/nothings/stb/master/stb_image.h ${CMAKE_BINARY_DIR}/stb/stb_image.h)
endif ()

if (NOT EXISTS ${CMAKE_BINARY_DIR}/stb/stb_image_resize.h)
    file(DOWNLOAD https://raw.githubusercontent.com/nothings/stb/master/stb_image_resize.h ${CMAKE_BINARY_DIR}/stb/stb_image_resize.h)
endif ()

# Build glad

if (DEBUG)
    set(GLAD_SRC_DIR "src/external/glad_debug")
else ()
    set(GLAD_SRC_DIR "src/external/glad")
endif ()

add_library(glad STATIC ${GLAD_SRC_DIR}/src/glad.c)
target_include_directories(glad PUBLIC ${GLAD_SRC_DIR}/include)

# Build project

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

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
        src/camera/Camera.h
        src/camera/CameraControls.h
        src/camera/CameraControls.cpp
        src/core/UpdateDelegate.h
        src/core/INode.h
        src/renderer/Texture.h
        src/renderer/Texture.cpp
        src/IoUtils.cpp
        src/IoUtils.h
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
        src/asset/Parser.cpp
        src/asset/Parser.h
        src/asset/LocalFileReader.cpp
        src/asset/LocalFileReader.h
        src/asset/Image.cpp
        src/asset/Image.h
        src/PixelFormat.h
        src/renderer/BlocksRenderer.cpp
        src/renderer/BlocksRenderer.h
        src/renderer/ChunkMesh.cpp
        src/renderer/ChunkMesh.h
        src/scene/WorldScene.cpp
        src/scene/WorldScene.h
        src/block/BlockRegistry.cpp
        src/block/BlockRegistry.h
        src/block/Block.h
        src/block/Block.cpp
        src/renderer/ViewFrustrum.h
        src/camera/Camera.cpp
        src/renderer/ViewFrustrum.cpp
        src/renderer/ViewFrustrum.cpp
        src/TypedContainer.h
        src/world/World.cpp
        src/world/World.h
        src/world/WorldGenerator.cpp
        src/world/WorldGenerator.h
        src/chunk/ChunkMeshData.h
        src/chunk/ChunkData.h
        src/chunk/ChunkMeshDataBuilder.cpp
        src/chunk/ChunkMeshDataBuilder.h
        src/chunk/Constants.h
        src/MathUtils.h
        src/ArrayUtils.h
        src/external/PerlinNoise.hpp
        src/asset/Asset.cpp
        src/Color.h
        src/renderer/TextureManager.cpp
        src/renderer/TextureManager.h
        src/renderer/TextureLookup.h
        src/asset/CmrcFileReader.h
        src/asset/CmrcFileReader.cpp src/chunk/ChunkBlocks.h src/chunk/ChunkBlocks.cpp)

set(LIBS SDL2::SDL2main SDL2::SDL2-static glad glm::glm spdlog::spdlog nlohmann_json nlohmann_json_schema_validator)

if (DEBUG)
    add_compile_definitions(DEBUG ASSETS_DIR="${PROJECT_SOURCE_DIR}/assets/")
else ()
    add_compile_definitions(USE_CMRC ASSETS_PREFIX="assets/")

    file(GLOB_RECURSE ASSETS ${PROJECT_SOURCE_DIR}/assets/*.*)
    string(REPLACE ${PROJECT_SOURCE_DIR}/ "" ASSETS "${ASSETS}")

    cmrc_add_resource_library(assets ${ASSETS})

    set(LIBS assets ${LIBS})
endif ()

configure_file(src/config.h.in ${CMAKE_BINARY_DIR}/src/config.h)

add_executable(${EXECUTABLE_TARGET_NAME} ${SRC})

target_link_libraries(${EXECUTABLE_TARGET_NAME} ${LIBS})
target_include_directories(${EXECUTABLE_TARGET_NAME} PRIVATE ${CMAKE_BINARY_DIR}/src ${CMAKE_BINARY_DIR}/stb ${CMAKE_BINARY_DIR}/json src src/external)