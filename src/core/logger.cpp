#include <stdio.h>

#include "logger.h"

const char *getStringForLogLevel(LogLevel level) {
    switch (level) {
    case LogLevel::FATAL:
        return "FATAL";
    case LogLevel::ERROR:
        return "ERROR";
    case LogLevel::WARNING:
        return "WARNING";
    case LogLevel::INFO:
        return "INFO";
    case LogLevel::DEBUG:
        return "DEBUG";
    case LogLevel::TRACE:
        return "TRACE";
    default:
        return "LOG";
    }
}

const char *getColorForLogLevel(LogLevel level) {
    switch (level) {
    case LogLevel::FATAL:
    case LogLevel::ERROR:
        return "41;30;";
    case LogLevel::WARNING:
        return "43;30;";
    case LogLevel::INFO:
        return "47;30;";
    case LogLevel::DEBUG:
        return "44;30;";
    case LogLevel::TRACE:
        return "45;30;";
    default:
        return "47:30;";
    }
}

void Log(LogLevel level, const char *message) {
    // TODO: @PLATFORM_DEPENDENT
    printf("\033[%s1m %s \033[0m %s\n", getColorForLogLevel(level), getStringForLogLevel(level),
           message);
}
