#define GLAD_GL_IMPLEMENTATION

#include <format>

#include "../core/logger.h"
#include "../core/math.h"
#include "../defines.h"
#include "../game/entity.h"

int uModelLoc;
int uViewProjLoc;

unsigned int initGraphics() {
    const char *vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec3 aNormal;
        layout (location = 2) in vec2 aUV;
        uniform mat4 uModel;
        uniform mat4 uViewProj;
        out vec3 vNormal;
        out vec2 vUV;
        void main() {
            gl_Position = uViewProj * uModel * vec4(aPos, 1.0);
            vNormal = mat3(transpose(inverse(uModel))) * aNormal;
            vUV = aUV;
        }
    )";
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        Log(LogLevel::FATAL, std::format("Vertex shader compilation failed: {}", infoLog).c_str());
    }

    const char *fragmentShaderSource = R"(
        #version 330 core
        in vec3 vNormal;
        out vec4 FragColor;
        void main() {
            vec3 n = normalize(vNormal);
            FragColor = vec4(n * 0.5 + 0.5, 1.0);
        }
    )";
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        Log(LogLevel::FATAL,
            std::format("Fragment shader compilation failed: {}", infoLog).c_str());
    }

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        Log(LogLevel::FATAL, std::format("Failed to link shader program: {}", infoLog).c_str());
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glBindVertexArray(0);

    glEnable(GL_DEPTH_TEST);

    uModelLoc = glGetUniformLocation(shaderProgram, "uModel");
    uViewProjLoc = glGetUniformLocation(shaderProgram, "uViewProj");

    return shaderProgram;
}

void shutdownGraphics(unsigned int shaderProgram) {
    glDeleteProgram(shaderProgram);
}

float toRadians(const float degrees) {
    return degrees * (3.141592 / 180);
}

void drawEntities(GLFWwindow *window, const std::vector<Entity *> &entities,
                  unsigned int shaderProgram, const MeshRegistry &registry) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderProgram);

    int w, h;
    glfwGetFramebufferSize(window, &w, &h);
    glViewport(0, 0, w, h);

    Mat4 proj = mat4_perspective(toRadians(90.0f), (float)w / (float)h, 0.1f, 100.0f);

    for (Entity *e : entities) {
        if (e->type == EntityType::Player) {
            Mat4 view =
                mat4_lookAt(Vector3{e->position.x, 7, e->position.z + 5}, e->position, {0, 1, 0});
            Mat4 viewProj = proj * view;
            glUniformMatrix4fv(uViewProjLoc, 1, GL_TRUE, &viewProj.entries[0][0]);
        }

        Mat4 model = mat4_translate(e->position) * mat4_scale(e->scale);
        glUniformMatrix4fv(uModelLoc, 1, GL_TRUE, &model.entries[0][0]);

        Mesh *m = registry.get(e->mesh);
        glBindVertexArray(m->VAO);
        if (m->indexCount > 0) {
            glDrawElements(GL_TRIANGLES, (GLsizei)m->indexCount, GL_UNSIGNED_INT, (void *)0);
        } else {
            glDrawArrays(GL_TRIANGLES, 0, (GLsizei)m->vertexCount);
        }
    }

    glBindVertexArray(0);

    glfwSwapBuffers(window);
    glfwPollEvents();
}
