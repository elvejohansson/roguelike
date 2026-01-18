#ifndef LOGGER_H
#define LOGGER_H

#include <cstdint>

enum class LogLevel : std::uint8_t {
    FATAL = 0,
    ERROR = 1,
    WARNING = 2,
    INFO = 3,
    DEBUG = 4,
    TRACE = 5,
};

void Log(LogLevel level, const char *message);

#endif
