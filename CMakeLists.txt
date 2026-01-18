cmake_minimum_required(VERSION 4.2)

project(Roguelike LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(ProjectOptions)
include(CompilerSettings)
include(Warnings)
include(Sanitizers)
include(Dependencies)
include(Testing)

add_subdirectory(src)

if (BUILD_TESTS)
    add_subdirectory(tests)
endif()
