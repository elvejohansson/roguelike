#ifndef PLATFORM_H
#define PLATFORM_H

#include "../core/logger.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#define PLATFORM_WINDOWS
#elif defined(__APPLE__)
#define PLATFORM_MAC
#elif defined(__linux__)
#define PLATFORM_LINUX
#endif

struct Platform;

struct PlatformWindow {
    bool shouldClose = false;
    int width, height;
    void *handle = nullptr;
};

struct PlatformWindowConfig {
    const char *title;
    int width, height;
};

struct PlatformAPI {
    bool (*init)(Platform *p);
    void (*shutdown)(Platform *p);

    double (*getTimeSeconds)(Platform *p);
    void (*sleepMs)(Platform *p, int ms);

    bool (*windowCreate)(Platform *p, const PlatformWindowConfig &config);
    void (*windowDestroy)(Platform *p);

    bool (*isKeyPressed)(Platform *p, int keyCode);

    void (*pumpEvents)(Platform *p);

    void (*log)(Platform *p, LogLevel level, const char *message);
};

struct Platform {
    PlatformAPI api;
    PlatformWindow *window = nullptr;
    void *state = nullptr;
};

bool platformInit(Platform *out);
void platformShutdown(Platform *p);

#endif
