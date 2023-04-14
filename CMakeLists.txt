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

set(CACHE SDL2_DISABLE_INSTALL ON)

FetchContent_MakeAvailable(SDL2 GLM spdlog glad cmrc)

add_subdirectory(${glad_SOURCE_DIR}/cmake ${glad_BINARY_DIR})

if (DEBUG)
    glad_add_library(glad_gl_core_33 REPRODUCIBLE DEBUG API gl:core=3.3)
else ()
    glad_add_library(glad_gl_core_33 REPRODUCIBLE API gl:core=3.3)
endif ()

if (NOT EXISTS ${CMAKE_BINARY_DIR}/stb_image.h)
    file(DOWNLOAD https://raw.githubusercontent.com/nothings/stb/master/stb_image.h ${CMAKE_BINARY_DIR}/stb/stb_image.h)
endif()

# Build project

set(EXECUTABLE_TARGET_NAME ShitCraft)

set(SRC ${CMAKE_BINARY_DIR}/src/config.h src/main.cpp src/Game.cpp src/include/Game.h src/include/config.h.in src/Shader.cpp src/include/Shader.h src/include/types.h src/include/Drawable.h src/include/EventConsumer.h src/include/EventConsumer.h src/Node.cpp src/include/Node.h src/include/NodeHost.h src/include/Node.h src/NodeHost.cpp src/Node.cpp src/include/Transform.h src/SimpleCube.cpp src/include/SimpleCube.h src/include/SimpleCubeScene.h src/include/Camera.h src/include/CameraControls.h src/CameraControls.cpp src/include/UpdateDelegate.h src/include/INode.h src/external/stb_image.cpp src/include/Texture.h src/Texture.cpp src/include/AssetManager.h src/AssetManager.cpp src/utils.cpp src/include/utils.h src/include/Context.h src/Context.cpp src/include/Vertex.h src/include/arch.h)
set(LIBS SDL2::SDL2main SDL2::SDL2-static glad_gl_core_33 glm::glm spdlog::spdlog)

if (DEBUG)
    set(RESOURCES_DIR ${PROJECT_SOURCE_DIR}/assets/)
else ()
    set(RES_FILES assets/glsl/basic.vert assets/glsl/basic.frag)
    set(RESOURCES_PREFIX assets/)
    set(LIBS assets ${LIBS})
    cmrc_add_resource_library(assets ${RES_FILES})
endif ()

configure_file(src/include/config.h.in ${CMAKE_BINARY_DIR}/src/config.h)

add_executable(${EXECUTABLE_TARGET_NAME} ${SRC})

target_link_libraries(${EXECUTABLE_TARGET_NAME} PRIVATE ${LIBS})
target_include_directories(${EXECUTABLE_TARGET_NAME} PRIVATE ${CMAKE_BINARY_DIR}/src ${CMAKE_BINARY_DIR}/stb src/include)