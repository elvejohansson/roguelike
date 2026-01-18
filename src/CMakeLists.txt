add_executable(Game
    core/logger.cpp
    main.cpp
)

target_link_libraries(Game PRIVATE
    project_options
    project_warnings
)
