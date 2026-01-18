include_guard(GLOBAL)

option(BUILD_TESTS           "Build tests" ON)
option(ENABLE_WARNNINGS      "Enable compiler warnings" ON)
option(WARNINGS_AS_ERRORS    "Treat compiler warnings as errors" OFF)

# Sanitize options
option(ENABLE_ASAN           "Enable AddressSanitizer" OFF)
option(ENABLE_UBSAN          "Enable UndefinedBehaviorSanitizer" OFF)
option(ENABLE_TSAN           "Enable ThreadSanitizer" OFF)

option(ENABLE_LTO            "Enable link-time optimization (IPO/LTO)" OFF)

option(ENABLE_SHADER_BUILD   "Compile shaders during build" ON)
option(ENABLE_ASSET_STAGING  "Copy/symlink assets next to binaries" ON)

add_library(project_options INTERFACE)
add_library(project_warnings INTERFACE)

target_compile_features(project_options INTERFACE cxx_std_23)

if (ENABLE_LTO)
    include(CheckIPOSupported)
    check_ipo_supported(RESULT ipo_ok OUTPUT ipo_err)
    if (ipo_ok)
        set_property(TARGET project_options PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
    else()
        message(WARNING "IPO/LTO requested but not supported: ${ipo_err}")
    endif()
endif()
