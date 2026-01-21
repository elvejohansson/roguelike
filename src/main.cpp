#include <format>
#include <fstream>

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

    std::ifstream file("../resources/cube.obj");
    if (!file.is_open()) {
        Log(LogLevel::FATAL, "File could not be opened");
        return -1;
    }

    std::string contents;
    std::string line;
    while (getline(file, line)) {
        contents += line + '\n';
    }

    file.close();

    MeshId mId = registry.add(makeMeshFromObj(contents));

    EntityManager manager;

    Entity *player = makeEntity(manager, EntityType::Player);
    player->position = Vector3{0, 0, 0};
    player->scale = Vector3{1, 1, 1};
    player->mesh = mId;

    Entity *enemy = makeEntity(manager, EntityType::Enemy);
    enemy->position = Vector3{5, 0, -5};
    enemy->scale = Vector3{1, 1, 1};
    enemy->mesh = mId;

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

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            player->position.z -= deltaTime * 1.0f;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            player->position.z += deltaTime * 1.0f;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            player->position.x += deltaTime * 1.0f;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            player->position.x -= deltaTime * 1.0f;
        }

        while (accumulator >= deltaTime) {
            // update()
            time += deltaTime;
            accumulator -= deltaTime;
        }

        const double alpha = accumulator / deltaTime;

        // render(window, alpha)

        drawEntities(window, manager.entities, shaderProgram, registry);
    }

    registry.clear();
    destroyAllEntities(manager);

    shutdownGraphics(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();
}
