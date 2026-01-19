#ifndef ASSERT_H
#define ASSERT_H

#include <format>

#include "logger.h"

// @PLATFORM_DEPENDENT
// TODO: __builtin_trap() does not work on MSVC, replace with __debugbreak() ?
// TODO: Make it possible to provide override error message if assertion fails?
// TODO: Ability to disable assertions, for release builds. We could also make
// assertions not call the trap function in release, just send an ERROR log
#define ASSERT(expr)                                                                               \
    {                                                                                              \
        if (expr) {                                                                                \
        } else {                                                                                   \
            Log(LogLevel::FATAL,                                                                   \
                std::format("Expression {} failed at {}:{}", #expr, __FILE__, __LINE__).c_str());  \
            __builtin_trap();                                                                      \
        }                                                                                          \
    }

#endif
