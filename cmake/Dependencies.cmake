include_guard(GLOBAL)

include(FetchContent)

option(USE_SYSTEM_DEPS "Prefer system-installed dependencies" ON)

# glad
add_library(dep_glad INTERFACE)
target_include_directories(dep_glad INTERFACE "${CMAKE_SOURCE_DIR}/vendor")
add_library(dep::glad ALIAS dep_glad)

# glfw
if(USE_SYSTEM_DEPS)
    find_package(glfw3 CONFIG REQUIRED)
    add_library(dep::glfw ALIAS glfw)
else()
    # fetchcontent
endif()
