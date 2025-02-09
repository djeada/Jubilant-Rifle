#ifndef GAME_CONTEXT_H
#define GAME_CONTEXT_H

#include "entities/bullet_pool.h"
#include "entities/enemy_array.h"
#include "entities/player.h"
#include "map/map.h"
#include "rendering/texture_manager.h"
#include <SDL2/SDL.h>

typedef struct GameContext {
  SDL_Renderer *renderer;
  TextureManager *texManager;
  Map *map;
  Player *player;
  EnemyArray *enemies;
  BulletPool *bulletPool;
} GameContext;

#endif // GAME_CONTEXT_H

