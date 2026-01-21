#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "../graphics/opengl.h"

#include <thread>

#include "../core/logger.h"
#include "input.h"
#include "platform.h"

static void errorCallback(int error, const char *description) {
    Log(LogLevel::ERROR, description);
}

static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

static void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
    PlatformWindow *pw = (PlatformWindow *)glfwGetWindowUserPointer(window);

    pw->width = width;
    pw->height = height;

    Log(LogLevel::DEBUG,
        std::format("[GLFW] Window was resized, got new dimensions {}x{}", width, height).c_str());
}

static void joystickCallback(int jid, int event) {
    switch (event) {
    case GLFW_CONNECTED: {
        Log(LogLevel::DEBUG,
            std::format("Joystick #{} connected ({})", jid, glfwGetGamepadName(jid)).c_str());
        return;
    }
    case GLFW_DISCONNECTED: {
        Log(LogLevel::DEBUG, std::format("Joystick #{} disconnected", jid).c_str());
        return;
    }
    default:
        return;
    }
}

struct State {};

bool linux_init(Platform *p) {
    p->state = new State{};
    return true;
}

void linux_shutdown(Platform *p) {
    if (p->state == nullptr) {
        return;
    }
    delete (State *)p->state;
    p->state = nullptr;
}

double linux_getTimeSeconds(Platform *p) {
    return glfwGetTime();
}

void linux_sleepMs(Platform *p, int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

bool linux_windowCreate(Platform *p, const PlatformWindowConfig &config) {
    if (!glfwInit()) {
        Log(LogLevel::FATAL, "Failed to initialize GLFW");
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // TODO: extract from config
    GLFWwindow *window = glfwCreateWindow(1280, 720, "Roguelike", NULL, NULL);
    if (!window) {
        Log(LogLevel::FATAL, "[GLFW] Window creation failed");
        glfwTerminate();
        return false;
    }

    PlatformWindow *_w = new PlatformWindow{};
    _w->handle = window;
    _w->width = 1280;
    _w->height = 720;
    p->window = _w;
    glfwSetWindowUserPointer(window, _w);

    glfwMakeContextCurrent(window);

    glfwSetErrorCallback(errorCallback);
    Log(LogLevel::DEBUG, "[GLFW] Registered error callback");

    glfwSetKeyCallback(window, keyCallback);
    Log(LogLevel::DEBUG, "[GLFW] Registered key callback");

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    Log(LogLevel::DEBUG, "[GLFW] Registered framebuffer size callback");

    glfwSetJoystickCallback(joystickCallback);
    Log(LogLevel::DEBUG, "[GLFW] Registered joystick callback");

    glbinding::initialize(glfwGetProcAddress);

    return true;
};

void linux_windowDestroy(Platform *p) {
    if (p->window == nullptr) {
        return;
    }

    PlatformWindow *pw = (PlatformWindow *)p->window;
    GLFWwindow *window = (GLFWwindow *)pw->handle;

    glfwDestroyWindow(window);
    glfwTerminate();
};

bool linux_isKeyPressed(Platform *p, int keyCode) {
    if (p->window == nullptr) {
        return false;
    }

    PlatformWindow *pw = (PlatformWindow *)p->window;
    GLFWwindow *window = (GLFWwindow *)pw->handle;

    return glfwGetKey(window, keyCode) == GLFW_PRESS;
}

float linux_getAxisValue(Platform *p, JoystickAxis axis) {
    // incredibly inefficient and fragile, works for now
    for (int jid = 0; jid <= GLFW_JOYSTICK_LAST; ++jid) {
        if (!glfwJoystickPresent(jid)) {
            continue;
        }

        int count;
        const float *axes = glfwGetJoystickAxes(jid, &count);

        switch (axis) {
        case JoystickAxis::LEFT_X:
            return axes[0];
        case JoystickAxis::LEFT_Y:
            return axes[1];
        case JoystickAxis::RIGHT_X:
            return axes[3];
        case JoystickAxis::RIGHT_Y:
            return axes[4];
        default:
            return 0.0F;
        }
    }
    return 0.0F;
}

void linux_pumpEvents(Platform *p) {
    if (p->window == nullptr) {
        return;
    }

    PlatformWindow *pw = (PlatformWindow *)p->window;
    GLFWwindow *window = (GLFWwindow *)pw->handle;

    // should be in renderer? or in a end-frame call maybe
    glfwSwapBuffers(window);

    glfwPollEvents();

    if (glfwWindowShouldClose(window) == 1) {
        pw->shouldClose = true;
    }
}

bool platformCreate_linux(Platform *out) {
    out->api = {};

    out->api.init = linux_init;
    out->api.shutdown = linux_shutdown;
    out->api.getTimeSeconds = linux_getTimeSeconds;
    out->api.sleepMs = linux_sleepMs;
    out->api.windowCreate = linux_windowCreate;
    out->api.windowDestroy = linux_windowDestroy;
    out->api.isKeyPressed = linux_isKeyPressed;
    out->api.getAxisValue = linux_getAxisValue;
    out->api.pumpEvents = linux_pumpEvents;

    return true;
}
