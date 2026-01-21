
if (WIN32)
    set(PLATFORM_SOURCES platform/platform_windows.cpp)
else()
    # this could be both mac and linux
    set(PLATFORM_SOURCES platform/platform_linux.cpp)
endif()


add_executable(Game
    core/logger.cpp
    core/math.h
    game/entity.cpp
    graphics/graphics.cpp
    graphics/mesh.cpp
    platform/platform.cpp
    ${PLATFORM_SOURCES}
    main.cpp
)

target_link_libraries(Game PRIVATE
    project_options
    project_warnings
    dep::glbinding
    dep::glfw
)
