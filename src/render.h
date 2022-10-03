#ifndef RENDER_H
#define RENDER_H

#include "humanoid.h"
#include "vector.h"

void renderAll(SDL_Renderer *renderer, Humanoid *player, Vector *enemies,
               Vector *bullets);

#endif