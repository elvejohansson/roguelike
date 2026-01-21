#include <cstddef>
#include <sstream>
#include <string>
#include <vector>

#include "../core/logger.h"
#include "../defines.h"
#include "mesh.h"

Mesh *makeMesh(const Vertex *vertices, unsigned int vertexCount) {
    Mesh *m = new Mesh;
    m->vertexCount = vertexCount;
    m->indexCount = 0;

    glGenVertexArrays(1, &m->VAO);
    glBindVertexArray(m->VAO);

    glGenBuffers(1, &m->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m->VBO);
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

Mesh *makeMesh(const Vertex *vertices, unsigned int vertexCount, const unsigned int *indices,
               unsigned int indexCount) {
    Mesh *m = new Mesh;
    m->vertexCount = vertexCount;
    m->indexCount = indexCount;

    glGenVertexArrays(1, &m->VAO);
    glBindVertexArray(m->VAO);

    glGenBuffers(1, &m->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m->VBO);
    glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(vertexCount * sizeof(Vertex)), vertices,
                 GL_STATIC_DRAW);

    glGenBuffers(1, &m->EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->EBO);
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

struct ObjVec3 {
    float x;
    float y;
    float z;
};

struct ObjVec2 {
    float u;
    float v;
};

struct ObjIndex {
    int v = -1;
    int vt = -1;
    int vn = -1;
};

int resolveIndex(int index, int count) {
    if (index > 0) {
        return index - 1;
    }
    if (index < 0) {
        return count + index;
    }
    return -1;
}

ObjIndex parseFaceIndex(const std::string &token, int posCount, int uvCount, int normCount) {
    ObjIndex out;
    std::stringstream ss(token);
    std::string part;
    int partIndex = 0;

    while (std::getline(ss, part, '/')) {
        if (!part.empty()) {
            int value = std::stoi(part);
            if (partIndex == 0) {
                out.v = resolveIndex(value, posCount);
            } else if (partIndex == 1) {
                out.vt = resolveIndex(value, uvCount);
            } else if (partIndex == 2) {
                out.vn = resolveIndex(value, normCount);
            }
        }
        partIndex++;
    }

    return out;
}

Vertex makeVertex(const ObjIndex &index, const std::vector<ObjVec3> &positions,
                  const std::vector<ObjVec3> &normals, const std::vector<ObjVec2> &uvs) {
    Vertex v{};

    if (index.v >= 0 && index.v < static_cast<int>(positions.size())) {
        const ObjVec3 &p = positions[static_cast<size_t>(index.v)];
        v.px = p.x;
        v.py = p.y;
        v.pz = p.z;
    }

    if (index.vn >= 0 && index.vn < static_cast<int>(normals.size())) {
        const ObjVec3 &n = normals[static_cast<size_t>(index.vn)];
        v.nx = n.x;
        v.ny = n.y;
        v.nz = n.z;
    }

    if (index.vt >= 0 && index.vt < static_cast<int>(uvs.size())) {
        const ObjVec2 &t = uvs[static_cast<size_t>(index.vt)];
        v.u = t.u;
        v.v = t.v;
    }

    return v;
}

Mesh *makeMeshFromObj(std::string source) {
    std::vector<ObjVec3> positions;
    std::vector<ObjVec3> normals;
    std::vector<ObjVec2> uvs;
    std::vector<Vertex> vertices;

    std::istringstream lines(source);
    std::string current;

    while (std::getline(lines, current)) {
        std::stringstream ss(current);
        std::string type;
        ss >> type;

        if (type == "v") {
            ObjVec3 p{};
            ss >> p.x >> p.y >> p.z;
            positions.push_back(p);
        } else if (type == "vn") {
            ObjVec3 n{};
            ss >> n.x >> n.y >> n.z;
            normals.push_back(n);
        } else if (type == "vt") {
            ObjVec2 t{};
            ss >> t.u >> t.v;
            uvs.push_back(t);
        } else if (type == "f") {
            std::vector<ObjIndex> face;
            std::string token;
            while (ss >> token) {
                face.push_back(parseFaceIndex(token, static_cast<int>(positions.size()),
                                              static_cast<int>(uvs.size()),
                                              static_cast<int>(normals.size())));
            }

            if (face.size() < 3) {
                continue;
            }

            for (size_t i = 1; i + 1 < face.size(); ++i) {
                vertices.push_back(makeVertex(face[0], positions, normals, uvs));
                vertices.push_back(makeVertex(face[i], positions, normals, uvs));
                vertices.push_back(makeVertex(face[i + 1], positions, normals, uvs));
            }
        }
    }

    if (vertices.empty()) {
        Log(LogLevel::ERROR, "OBJ had no faces to load");
    }

    Mesh *m = makeMesh(vertices.data(), static_cast<unsigned int>(vertices.size()));
    return m;
}
