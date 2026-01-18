include_guard(GLOBAL)

if (NOT BUILD_TESTS)
    return()
endif()

include(CTest)
enable_testing()

# pull catch2
include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.12.0
)
FetchContent_MakeAvailable(Catch2)

function(add_game_test name)
    set(options)
    set(oneValueArgs LABEL)
    set(multiValueArgs LABELS SOURCES)
    cmake_parse_arguments(T "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if (NOT T_SOURCES)
        message(FATAL_ERROR "add_game_test(${name}) missing SOURCES ...")
    endif()

    add_executable(${name} ${T_SOURCES})
    target_link_libraries(${name} PRIVATE project_options project_warnings Catch2::Catch2WithMain)

    add_test(NAME ${name} COMMAND ${name})

    set(_labels "")
    if (T_LABEL)
        list(APPEND _labels "${T_LABEL}")
    endif()
    if (T_LABELS)
        list(APPEND _labels ${T_LABELS})
    endif()
    if (_labels)
        set_tests_properties(${name} PROPERTIES LABELS "${_labels}")
    endif()

    set_tests_properties(${name} PROPERTIES TIMEOUT 30)
endfunction()
