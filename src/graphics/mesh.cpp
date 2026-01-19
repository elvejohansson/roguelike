#include <cstddef>

#include "../defines.h"
#include "mesh.h"

Mesh makeMesh(const Vertex *vertices, unsigned int vertexCount) {
    Mesh m{};
    m.vertexCount = vertexCount;
    m.indexCount = 0;

    glGenVertexArrays(1, &m.VAO);
    glBindVertexArray(m.VAO);

    glGenBuffers(1, &m.VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m.VBO);
    glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(vertexCount * sizeof(Vertex)), vertices,
                 GL_STATIC_DRAW);

    // location 0: position (vec3)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, px));
    glEnableVertexAttribArray(0);

    // location 1: normal (vec3)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, nx));
    glEnableVertexAttribArray(1);

    // location 2: uv (vec2)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, u));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
    return m;
}

Mesh makeMesh(const Vertex *vertices, unsigned int vertexCount, const unsigned int *indices,
              unsigned int indexCount) {
    Mesh m{};
    m.vertexCount = vertexCount;
    m.indexCount = indexCount;

    glGenVertexArrays(1, &m.VAO);
    glBindVertexArray(m.VAO);

    glGenBuffers(1, &m.VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m.VBO);
    glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(vertexCount * sizeof(Vertex)), vertices,
                 GL_STATIC_DRAW);

    glGenBuffers(1, &m.EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(indexCount * sizeof(unsigned int)), indices,
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, px));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, nx));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, u));
    glEnableVertexAttribArray(2);

    // IMPORTANT: do NOT unbind GL_ELEMENT_ARRAY_BUFFER while VAO is bound.
    // Binding 0 to EBO here would detach it from the VAO.
    glBindVertexArray(0);

    return m;
}
