#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include "rendering/render.h"

// Runs the main game loop (which includes switching between menu and game)
void runGame(SDL_Renderer *renderer);

#endif // GAME_H
