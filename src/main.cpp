#include <format>
#include <fstream>

#include "core/assert.h"
#include "core/logger.h"
#include "core/math.h"
#include "game/entity.h"
#include "graphics/graphics.h"
#include "graphics/mesh.h"
#include "platform/platform.h"

int main(void) {
    Platform platform;
    if (!platformInit(&platform)) {
        return -1;
    }

    if (!platform.api.windowCreate(&platform, {})) {
        return -1;
    }

    PlatformWindow *window = platform.window;

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

    double currentTime = platform.api.getTimeSeconds(&platform);
    double accumulator = 0.0;

    while (!window->shouldClose) {
        double newTime = platform.api.getTimeSeconds(&platform);
        double frameTime = newTime - currentTime;
        currentTime = newTime;

        if (frameTime > 0.25) {
            frameTime = 0.25;
        }

        accumulator += frameTime;

        if (platform.api.isKeyPressed(&platform, 87)) {
            player->position.z -= deltaTime * 1.0f;
        }
        if (platform.api.isKeyPressed(&platform, 83)) {
            player->position.z += deltaTime * 1.0f;
        }
        if (platform.api.isKeyPressed(&platform, 68)) {
            player->position.x += deltaTime * 1.0f;
        }
        if (platform.api.isKeyPressed(&platform, 65)) {
            player->position.x -= deltaTime * 1.0f;
        }

        while (accumulator >= deltaTime) {
            // update()
            time += deltaTime;
            accumulator -= deltaTime;
        }

        const double alpha = accumulator / deltaTime;

        // render(window, alpha)

        drawEntities(manager.entities, shaderProgram, registry, window->width, window->height);

        platform.api.pumpEvents(&platform);
    }

    registry.clear();
    destroyAllEntities(manager);

    shutdownGraphics(shaderProgram);
}
