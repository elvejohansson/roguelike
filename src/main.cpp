#define GLFW_INCLUDE_NONE
#define GLAD_GL_IMPLEMENTATION
#include <GLFW/glfw3.h>
#include <glad/gl.h>

#include <format>

#include "core/assert.h"
#include "core/logger.h"
#include "core/math.h"

static void errorCallback(int error, const char *description) {
    Log(LogLevel::ERROR, description);
}

static void keyCallback(GLFWwindow *window, int key, int scancode, int action,
                        int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

static void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
    Log(LogLevel::DEBUG,
        std::format("[GLFW] Window was resized, got new dimensions {}x{}",
                    width, height)
            .c_str());
    glViewport(0, 0, width, height);
}

void update(double time, double deltaTime) {
    glfwPollEvents();
}

void render(GLFWwindow *window) {
    glfwSwapBuffers(window);
}

int main(void) {
    if (!glfwInit()) {
        Log(LogLevel::FATAL, "Failed to initialize GLFW");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(1280, 720, "Roguelike", NULL, NULL);
    if (!window) {
        Log(LogLevel::FATAL, "[GLFW] Window creation failed");
        return -1;
    }

    glfwMakeContextCurrent(window);

    glfwSetErrorCallback(errorCallback);
    Log(LogLevel::DEBUG, "[GLFW] Registered error callback");

    glfwSetKeyCallback(window, keyCallback);
    Log(LogLevel::DEBUG, "[GLFW] Registered key callback");

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    Log(LogLevel::DEBUG, "[GLFW] Registered framebuffer size callback");

    int version = gladLoadGL(glfwGetProcAddress);
    if (version == 0) {
        Log(LogLevel::FATAL, "[GL] Failed to initialize context");
        return -1;
    }

    Log(LogLevel::INFO,
        std::format("[GL] Loaded OpenGL version {}.{}",
                    GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version))
            .c_str());

    double time = 0.0;
    double deltaTime = 0.01;

    double currentTime = glfwGetTime();
    double accumulator = 0.0;

    while (!glfwWindowShouldClose(window)) {
        double newTime = glfwGetTime();
        double frameTime = newTime - currentTime;
        currentTime = newTime;

        if (frameTime > 0.25) {
            frameTime = 0.25;
        }

        accumulator += frameTime;

        while (accumulator >= deltaTime) {
            update(time, deltaTime);
            time += deltaTime;
            accumulator -= deltaTime;
        }

        const double alpha = accumulator / deltaTime;

        render(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}
