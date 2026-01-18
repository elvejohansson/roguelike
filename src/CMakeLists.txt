add_executable(Game
    core/logger.cpp
    core/math.h
    main.cpp
)

target_link_libraries(Game PRIVATE
    project_options
    project_warnings
    dep::glad
    dep::glfw
)
