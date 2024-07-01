#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include "entities/humanoid.h"
#include "entities/player.h"
#include <SDL2/SDL.h>
#include <stdbool.h>

bool processEvents(SDL_Window *window, Player *player);

#endif // EVENT_HANDLER_H
