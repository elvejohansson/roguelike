#ifndef MESH_H
#define MESH_H

#include <unordered_map>

typedef unsigned int MeshId;

struct Mesh {
    unsigned int VAO, VBO, EBO = 0;
    unsigned int vertexCount = 0;
    unsigned int indexCount = 0;
};

struct MeshRegistry {
    std::unordered_map<MeshId, Mesh> meshes;
    MeshId current = 0;

    MeshId add(const Mesh &mesh) {
        MeshId id = current++;
        meshes[id] = mesh;
        return id;
    }

    const Mesh &get(MeshId id) const {
        return meshes.at(id);
    }
};

struct Vertex {
    float px, py, pz;
    float nx, ny, nz;
    float u, v;
};

Mesh makeMesh(const Vertex *vertices, unsigned int vertexCount);
Mesh makeMesh(const Vertex *vertices, unsigned int vertexCount,
              const unsigned int *indices, unsigned int indexCount);

#endif
