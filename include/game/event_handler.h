#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include "entities/bullet_pool.h"
#include "entities/grenade_pool.h"
#include "entities/player.h"
#include "game/game_state.h"
#include <SDL2/SDL.h>
#include <stdbool.h>

// Processes pending events, updating the player and game state.
// The bullet pool and grenade pool pointers are provided for shooting/throwing.
void handleGameEvents(SDL_Event *e, Player *player, GameState *gameState,
                      bool *gameRunning, BulletPool *bulletPool);

// Extended event handler with grenade support
void handleGameEventsExtended(SDL_Event *e, Player *player, GameState *gameState,
                               bool *gameRunning, BulletPool *bulletPool,
                               GrenadePool *grenadePool);

#endif // EVENT_HANDLER_H

