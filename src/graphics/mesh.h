#ifndef MESH_H
#define MESH_H

#include <format>
#include <unordered_map>

#include "../core/logger.h"

typedef unsigned int MeshId;

struct Mesh {
    unsigned int VAO, VBO, EBO = 0;
    unsigned int vertexCount = 0;
    unsigned int indexCount = 0;
};

struct MeshRegistry {
    std::unordered_map<MeshId, Mesh *> meshes;
    MeshId current = 0;

    MeshId add(Mesh *mesh) {
        MeshId id = current++;
        meshes[id] = mesh;
        return id;
    }

    Mesh *get(MeshId id) const {
        return meshes.at(id);
    }

    void clear() const {
        for (auto &[id, mesh] : meshes) {
            free(mesh);
            Log(LogLevel::DEBUG, std::format("Freed mesh #{}", id).c_str());
        }
    }
};

struct Vertex {
    float px, py, pz;
    float nx, ny, nz;
    float u, v;
};

Mesh *makeMesh(const Vertex *vertices, unsigned int vertexCount);
Mesh *makeMesh(const Vertex *vertices, unsigned int vertexCount, const unsigned int *indices,
               unsigned int indexCount);
Mesh *makeMeshFromObj(std::string source);

#endif
