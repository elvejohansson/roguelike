include_guard(GLOBAL)

include(FetchContent)

option(USE_SYSTEM_DEPS "Prefer system-installed dependencies" ON)

# glbinding
if(USE_SYSTEM_DEPS)
    find_package(glbinding CONFIG REQUIRED)
    add_library(dep::glbinding ALIAS glbinding::glbinding)
else()
    # fetchcontent
endif()

# glfw
if(USE_SYSTEM_DEPS)
    find_package(glfw3 CONFIG REQUIRED)
    add_library(dep::glfw ALIAS glfw)
else()
    # fetchcontent
endif()
