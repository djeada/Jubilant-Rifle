#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include "entities/bullet_pool.h"
#include "entities/player.h"
#include "game/game_state.h"
#include <SDL2/SDL.h>
#include <stdbool.h>

// Processes pending events, updating the player and game state.
// The bullet pool pointer is provided so that the player can shoot.
void handleGameEvents(SDL_Event *e, Player *player, GameState *gameState,
                      bool *gameRunning, BulletPool *bulletPool);

#endif // EVENT_HANDLER_H

