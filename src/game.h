#ifndef GAME_H
#define GAME_H

#include "render.h"

bool processEvents(SDL_Window *window, Humanoid *man);
void updateLogic(Map *map, Humanoid *man, Vector *enemies);
void runGame();

#endif
