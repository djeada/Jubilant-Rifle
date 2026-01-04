#ifndef GAME_CONTEXT_H
#define GAME_CONTEXT_H

#include "entities/bullet_pool.h"
#include "entities/enemy_array.h"
#include "entities/grenade_pool.h"
#include "entities/player.h"
#include "map/map.h"
#include "rendering/texture_manager.h"
#include "game/score.h"
#include <SDL2/SDL.h>

typedef struct GameContext {
  SDL_Renderer *renderer;
  TextureManager *texManager;
  Map *map;
  Player *player;
  EnemyArray *enemies;
  BulletPool *bulletPool;
  GrenadePool *grenadePool;    /**< Pool for active grenades */
  ScoreTracker *scoreTracker;  /**< Score and timer tracking */
  float parachuteSpawnTimer;   /**< Timer for spawning parachute enemies */
  float parachuteSpawnInterval; /**< Interval between parachute spawns */
  int parachuteWaveCount;      /**< Number of parachute enemies per wave */
} GameContext;

#endif // GAME_CONTEXT_H

