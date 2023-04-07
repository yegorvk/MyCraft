cmake_minimum_required(VERSION 3.24)
project(ShitCraft VERSION 0.0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)

if (${CMAKE_BUILD_TYPE} STREQUAL Debug)
    set(BUILD_TYPE_DEBUG 1)
else ()
    set(BUILD_TYPE_DEBUG 0)
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
glad_add_library(glad_gl_core_33 REPRODUCIBLE API gl:core=3.3)

# Build project

set(EXECUTABLE_TARGET_NAME ShitCraft)

configure_file(src/config.h.in ${CMAKE_BINARY_DIR}/src/config.h)

set(SRC ${CMAKE_BINARY_DIR}/src/config.h src/main.cpp src/Game/Game.cpp src/Game/Game.h src/config.h.in  src/Shader/Shader.cpp src/Shader/Shader.h src/types.h src/resources.h src/resources.cpp)

add_executable(${EXECUTABLE_TARGET_NAME} ${SRC})
target_link_libraries(${EXECUTABLE_TARGET_NAME} PRIVATE SDL2::SDL2main SDL2::SDL2-static glad_gl_core_33 glm::glm spdlog::spdlog)
target_include_directories(${EXECUTABLE_TARGET_NAME} PRIVATE ${CMAKE_BINARY_DIR}/src src)