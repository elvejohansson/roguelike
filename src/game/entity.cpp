#include <algorithm>

#include "entity.h"

const std::string getEntityTypeStr(const EntityType type) {
    switch (type) {
    case EntityType::Player:
        return "Player";
    case EntityType::Enemy:
        return "Enemy";
    default:
        return "ENTITY";
    }
}

Entity *makeEntity(EntityManager &manager, EntityType type) {
    Entity *e = new Entity{};

    e->id = manager.currentId++;
    e->type = type;
    e->position = Vector3{0, 0, 0};

    manager.entities.push_back(e);
    manager.entityMap[e->id] = e;
    manager.entityTypeMap[type].push_back(e);

    return e;
}

void destroyEntity(EntityManager &manager, EntityId id) {
    auto it = manager.entityMap.find(id);
    if (it == manager.entityMap.end())
        return;

    Entity *e = it->second;

    auto vit = std::find(manager.entities.begin(), manager.entities.end(), e);
    if (vit != manager.entities.end())
        manager.entities.erase(vit);

    auto &tv = manager.entityTypeMap[e->type];
    auto tit = std::find(tv.begin(), tv.end(), e);
    if (tit != tv.end())
        tv.erase(tit);

    manager.entityMap.erase(it);
    delete e;
}

void destroyAllEntities(EntityManager &manager) {
    for (Entity *entity : manager.entities) {
        delete entity;
    }

    manager.entities.clear();
    manager.entityMap.clear();
    manager.entityTypeMap.clear();
}

Entity *getEntityById(const EntityManager &manager, EntityId id) {
    auto it = manager.entityMap.find(id);
    return (it == manager.entityMap.end()) ? nullptr : it->second;
}

std::vector<Entity *> *getEntitiesByTag(EntityManager &manager,
                                        EntityType type) {
    return &manager.entityTypeMap[type];
}
