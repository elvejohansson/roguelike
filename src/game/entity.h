#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <unordered_map>
#include <vector>

#include "../core/math.h"
#include "../graphics/mesh.h"

enum EntityType {
    Player,
    Enemy,
};

const std::string getEntityTypeStr(const EntityType type);

typedef unsigned int EntityId;

struct Entity {
    EntityId id;
    EntityType type;
    Vector3 position;
    Vector3 scale;

    MeshId mesh = 0;
};

struct EntityManager {
    std::vector<Entity *> entities;
    std::unordered_map<EntityId, Entity *> entityMap;
    std::unordered_map<EntityType, std::vector<Entity *>> entityTypeMap;
    EntityId currentId = 0;
};

[[nodiscard]] Entity *makeEntity(EntityManager &manager, EntityType type);
void destroyEntity(EntityManager &manager, EntityId id);
void destroyAllEntities(EntityManager &manager);
[[nodiscard]] Entity *getEntityById(const EntityManager &manager, EntityId id);
[[nodiscard]] std::vector<Entity *> *
getEntitiesByTag(const EntityManager &manager, EntityType type);

#endif
