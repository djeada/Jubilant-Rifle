#ifndef RENDER_H
#define RENDER_H

#include "entities/humanoid.h"
#include "map/map.h"
#include "utils/resources.h"
#include "utils/vector.h"

typedef struct Camera {
  int x, y;
  int width, height;
} Camera;

void centerCameraOnPlayer(Camera *camera, Humanoid *player);

void setRenderLogicalSize(SDL_Renderer *renderer, int windowWidth,
                          int windowHeight);
void render(SDL_Renderer *renderer, Map *map, Humanoid *player, Camera *camera);

#endif
