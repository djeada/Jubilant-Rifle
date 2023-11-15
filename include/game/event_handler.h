#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include "entities/humanoid.h"
#include <SDL2/SDL.h>
#include <stdbool.h>

// Declare the processEvents function, which is responsible for handling events.
bool processEvents(SDL_Window *window, Humanoid *player);

#endif // EVENT_HANDLER_H
