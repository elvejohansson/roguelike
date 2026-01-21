#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "../game/entity.h"
#include "mesh.h"

unsigned int initGraphics();
void shutdownGraphics(unsigned int shaderProgram);

void drawEntities(const std::vector<Entity *> &entities, unsigned int shaderProgram,
                  const MeshRegistry &registry, int width, int height);

#endif
