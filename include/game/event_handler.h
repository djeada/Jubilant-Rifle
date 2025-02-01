#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "entities/entity.h"
#include "game/game_state.h"
#include "entities/bullet_pool.h"  // for bullet_pool_spawn()

// Processes pending events, updating the player and game state.
// The bullet pool pointer is provided so that the player can shoot.
void handleGameEvents(SDL_Event *e, Entity *player, GameState *gameState, bool *gameRunning, BulletPool *bulletPool);

#endif // EVENT_HANDLER_H
