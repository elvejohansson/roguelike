#include <format>

#include "core/assert.h"
#include "core/logger.h"
#include "core/math.h"
#include "defines.h"
#include "game/entity.h"
#include "graphics/graphics.h"
#include "graphics/mesh.h"

static void errorCallback(int error, const char *description) {
    Log(LogLevel::ERROR, description);
}

static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

static void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
    Log(LogLevel::DEBUG,
        std::format("[GLFW] Window was resized, got new dimensions {}x{}", width, height).c_str());
    glViewport(0, 0, width, height);
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
        glfwTerminate();
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
        glfwTerminate();
        return -1;
    }

    Log(LogLevel::INFO, std::format("[GL] Loaded OpenGL version {}.{}", GLAD_VERSION_MAJOR(version),
                                    GLAD_VERSION_MINOR(version))
                            .c_str());

    unsigned int shaderProgram = initGraphics();

    MeshRegistry registry;

    Vertex cubeVerts[] = {
        // +Z (front)
        {-0.5f, -0.5f, 0.5f, 0, 0, 1, 0, 0},
        {0.5f, -0.5f, 0.5f, 0, 0, 1, 1, 0},
        {0.5f, 0.5f, 0.5f, 0, 0, 1, 1, 1},
        {-0.5f, 0.5f, 0.5f, 0, 0, 1, 0, 1},

        // -Z (back)
        {0.5f, -0.5f, -0.5f, 0, 0, -1, 0, 0},
        {-0.5f, -0.5f, -0.5f, 0, 0, -1, 1, 0},
        {-0.5f, 0.5f, -0.5f, 0, 0, -1, 1, 1},
        {0.5f, 0.5f, -0.5f, 0, 0, -1, 0, 1},

        // +X (right)
        {0.5f, -0.5f, 0.5f, 1, 0, 0, 0, 0},
        {0.5f, -0.5f, -0.5f, 1, 0, 0, 1, 0},
        {0.5f, 0.5f, -0.5f, 1, 0, 0, 1, 1},
        {0.5f, 0.5f, 0.5f, 1, 0, 0, 0, 1},

        // -X (left)
        {-0.5f, -0.5f, -0.5f, -1, 0, 0, 0, 0},
        {-0.5f, -0.5f, 0.5f, -1, 0, 0, 1, 0},
        {-0.5f, 0.5f, 0.5f, -1, 0, 0, 1, 1},
        {-0.5f, 0.5f, -0.5f, -1, 0, 0, 0, 1},

        // +Y (top)
        {-0.5f, 0.5f, 0.5f, 0, 1, 0, 0, 0},
        {0.5f, 0.5f, 0.5f, 0, 1, 0, 1, 0},
        {0.5f, 0.5f, -0.5f, 0, 1, 0, 1, 1},
        {-0.5f, 0.5f, -0.5f, 0, 1, 0, 0, 1},

        // -Y (bottom)
        {-0.5f, -0.5f, -0.5f, 0, -1, 0, 0, 0},
        {0.5f, -0.5f, -0.5f, 0, -1, 0, 1, 0},
        {0.5f, -0.5f, 0.5f, 0, -1, 0, 1, 1},
        {-0.5f, -0.5f, 0.5f, 0, -1, 0, 0, 1},
    };

    unsigned int cubeIdx[] = {
        0,  1,  2,  0,  2,  3,  // front
        4,  5,  6,  4,  6,  7,  // back
        8,  9,  10, 8,  10, 11, // right
        12, 13, 14, 12, 14, 15, // left
        16, 17, 18, 16, 18, 19, // top
        20, 21, 22, 20, 22, 23  // bottom
    };

    MeshId mId = registry.add(makeMesh(cubeVerts, 24, cubeIdx, 36));

    EntityManager manager;

    Entity *player = makeEntity(manager, EntityType::Player);
    player->position = Vector3{0, 0, 0};
    player->scale = Vector3{1, 1, 1};
    player->mesh = mId;

    double time = 0.0;
    double deltaTime = 1.0 / 60.0; // 60HZ

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
            // update()
            time += deltaTime;
            accumulator -= deltaTime;
        }

        const double alpha = accumulator / deltaTime;

        // render(window, alpha)

        drawEntities(window, manager.entities, shaderProgram, registry);
    }

    destroyAllEntities(manager);

    shutdownGraphics(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();
}
