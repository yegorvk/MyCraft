cmake_minimum_required(VERSION 3.24)
project(SimplexNoise LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

#if (MSVC)
#    set(DEV_NULL "NUL")
#    set(SYSTEM_LIBRARIES "")
#else (MSVC)
#    set(DEV_NULL "/dev/null")
#    if (NOT APPLE)
#        set(SYSTEM_LIBRARIES "rt")
#    endif (NOT APPLE)
#
#    if (CMAKE_COMPILER_IS_GNUCXX)
#        add_definitions(-export-dynamic)
#    endif (CMAKE_COMPILER_IS_GNUCXX)
#endif (MSVC)

add_library(SimplexNoise SHARED SimplexNoise.cpp SimplexNoise.h)
target_include_directories(SimplexNoise INTERFACE ${PROJECT_SOURCE_DIR})