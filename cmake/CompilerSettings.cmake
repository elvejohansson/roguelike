include_guard(GLOBAL)

if (MSVC)
    # fill in for windows build
    # /MP /EHsc /Zc:__cplusplus NOMINMAX WIN32_LEAN_AND_MEAN
else()
endif()
