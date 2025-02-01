#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "entities/entity.h"
#include "game/game_state.h"

// Processes all pending events and updates the player and game state accordingly.
void handleGameEvents(SDL_Event *e, Entity *player, GameState *gameState, bool *gameRunning);

#endif // EVENT_HANDLER_H
