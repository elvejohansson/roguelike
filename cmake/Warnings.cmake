include_guard(GLOBAL)

if (NOT ENABLE_WARNINGS)
    return()
endif()

if (MSVC)
    target_compile_options(project_warnings INTERFACE /W4)

    if (WARNINGS_AS_ERRORS)
        target_compile_options(project_warnings INTERFACE /WX)
    endif()
else()
    target_compile_options(project_warnings INTERFACE
        -Wall -Wextra -Wpedantic
        -Wconversion -Wsign-conversion
    )

    if (WARNINGS_AS_ERRORS)
        target_compile_options(project_warnings INTERFACE -Werror)
    endif()

    if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        target_compile_options(project_warnings INTERFACE -fcolor-diagnostics)
    endif()
endif()

