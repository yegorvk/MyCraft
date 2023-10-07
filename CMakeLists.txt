cmake_minimum_required(VERSION 3.22.1)
project(ShitCraft VERSION 0.0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)

set(BUILD_SHARED_LIBS OFF)

if (NOT DEFINED BUNDLE_RESOURCES)
    if ((NOT DEFINED CMAKE_BUILD_TYPE) OR (${CMAKE_BUILD_TYPE} STREQUAL Release))
        set(BUNDLE_RESOURCES ON)
    else ()
        set(BUNDLE_RESOURCES OFF)
    endif ()
endif ()

# Resolve dependencies using FetchContent

include(FetchContent)

FetchContent_Declare(cmrc URL https://github.com/vector-of-bool/cmrc/archive/refs/tags/2.0.1.tar.gz)
FetchContent_Declare(SDL2 URL https://github.com/libsdl-org/SDL/archive/refs/tags/release-2.26.5.tar.gz)
FetchContent_Declare(GLM URL https://github.com/g-truc/glm/archive/refs/tags/0.9.9.8.tar.gz)
FetchContent_Declare(spdlog URL https://github.com/gabime/spdlog/archive/refs/tags/v1.11.0.tar.gz)
FetchContent_Declare(nlohmann_json URL https://github.com/nlohmann/json/archive/refs/tags/v3.11.2.tar.gz)
FetchContent_Declare(nlohmann_json_schema_validator URL https://github.com/pboettch/json-schema-validator/archive/refs/tags/2.2.0.zip)
FetchContent_Declare(readerwriterqueue GIT_REPOSITORY https://github.com/cameron314/readerwriterqueue GIT_TAG master)

set(SDL2_DISABLE_INSTALL ON CACHE BOOL "" FORCE)
set(JSON_VALIDATOR_INSTALL OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(cmrc SDL2 GLM spdlog nlohmann_json nlohmann_json_schema_validator readerwriterqueue)

if (NOT EXISTS ${CMAKE_BINARY_DIR}/stb/stb_image.h)
    file(DOWNLOAD https://raw.githubusercontent.com/nothings/stb/master/stb_image.h ${CMAKE_BINARY_DIR}/stb/stb_image.h)
endif ()

if (NOT EXISTS ${CMAKE_BINARY_DIR}/stb/stb_image_resize.h)
    file(DOWNLOAD https://raw.githubusercontent.com/nothings/stb/master/stb_image_resize.h ${CMAKE_BINARY_DIR}/stb/stb_image_resize.h)
endif ()

# Build glad

set(GLAD_SRC_DIR ${PROJECT_SOURCE_DIR}/src/external/$<IF:$<CONFIG:Debug>,glad_debug,glad>)
add_library(glad STATIC ${GLAD_SRC_DIR}/src/glad.c)
target_include_directories(glad PUBLIC ${GLAD_SRC_DIR}/include)

# Build project

set(EXECUTABLE_TARGET_NAME ${PROJECT_NAME})
set(LIBS SDL2::SDL2main SDL2::SDL2-static glad glm::glm spdlog::spdlog nlohmann_json nlohmann_json_schema_validator readerwriterqueue)

set(SRC ${CMAKE_BINARY_DIR}/src/config.h
        src/external/BS_thread_pool.hpp
        src/external/stb.cpp
        src/external/PerlinNoise.hpp
        src/main.cpp
        src/Game.cpp
        src/Game.h
        src/config.h.in
        src/gl/Shader.cpp
        src/gl/Shader.h
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
        src/camera/CameraControls.h
        src/camera/CameraControls.cpp
        src/core/UpdateDelegate.h
        src/core/INode.h
        src/gl/Texture.h
        src/gl/Texture.cpp
        src/utils/IoUtils.cpp
        src/utils/IoUtils.h
        src/GameContext.h
        src/GameContext.cpp
        src/arch.h
        src/asset/Provider.h
        src/asset/Manager.cpp
        src/asset/Manager.h
        src/asset/Asset.h
        src/gl/ShaderType.h
        src/asset/Cache.h
        src/asset/FileReader.h
        src/asset/Index.h
        src/asset/Parser.cpp
        src/asset/Parser.h
        src/asset/LocalFileReader.cpp
        src/asset/LocalFileReader.h
        src/asset/Image.cpp
        src/asset/Image.h
        src/renderer/ChunkRenderer.cpp
        src/renderer/ChunkRenderer.h
        src/mesh/ChunkMesh.cpp
        src/mesh/ChunkMesh.h
        src/scene/WorldScene.cpp
        src/scene/WorldScene.h
        src/registry/BlockRegistry.cpp
        src/registry/BlockRegistry.h
        src/block/Block.h
        src/block/Block.cpp
        src/renderer/ViewFrustrum.h
        src/renderer/ViewFrustrum.cpp
        src/camera/FreeCamera.h
        src/camera/FreeCamera.cpp
        src/world/World.cpp
        src/world/World.h
        src/world/WorldGenerator.cpp
        src/world/WorldGenerator.h
        src/chunk/ChunkMeshData.h
        src/chunk/ChunkData.h
        src/chunk/ChunkMeshDataBuilder.cpp
        src/chunk/ChunkMeshDataBuilder.h
        src/chunk/Constants.h
        src/utils/MathUtils.h
        src/utils/ArrayUtils.h
        src/asset/Asset.cpp
        src/registry/TextureManager.cpp
        src/registry/TextureManager.h
        src/registry/TextureLookup.h
        src/asset/CmrcFileReader.h
        src/asset/CmrcFileReader.cpp
        src/chunk/ChunkBlocks.h
        src/chunk/ChunkBlocks.cpp
        src/core/Executor.h
        src/core/Executor.cpp
        src/renderer/HUDRenderer.cpp
        src/renderer/HUDRenderer.h
        src/gl/GlResource.h
        src/renderer/CompositeRenderer.cpp
        src/renderer/CompositeRenderer.h
        src/gl/GlTypes.h
        src/mesh/RectMesh.cpp
        src/mesh/RectMesh.h
        src/utils/Utils.h
        src/utils/MathUtils.cpp
        src/mesh/SkyboxMesh.cpp
        src/mesh/SkyboxMesh.h
        src/mesh/SimpleMesh.cpp
        src/mesh/SimpleMesh.h
        src/gl/TextureDefinitions.h
        src/renderer/SkyRenderer.cpp
        src/renderer/SkyRenderer.h
        src/renderer/Perspective.h)

#add_compile_definitions()

add_compile_definitions($<$<CONFIG:Debug>:DEBUG>)

if (NOT BUNDLE_RESOURCES)
    add_compile_definitions(ASSETS_DIR="${PROJECT_SOURCE_DIR}/assets/")
else ()
    add_compile_definitions(USE_CMRC ASSETS_PREFIX="assets/")

    file(GLOB_RECURSE ASSETS ${PROJECT_SOURCE_DIR}/assets/*.*)
    string(REPLACE ${PROJECT_SOURCE_DIR}/ "" ASSETS "${ASSETS}")

    cmrc_add_resource_library(assets ${ASSETS})

    list(APPEND LIBS assets)
endif ()

configure_file(src/config.h.in ${CMAKE_BINARY_DIR}/src/config.h)

add_executable(${EXECUTABLE_TARGET_NAME} ${SRC})

target_link_libraries(${EXECUTABLE_TARGET_NAME} ${LIBS})
target_include_directories(${EXECUTABLE_TARGET_NAME} PRIVATE ${CMAKE_BINARY_DIR}/src ${CMAKE_BINARY_DIR}/stb ${CMAKE_BINARY_DIR}/json src src/external)