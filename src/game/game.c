#include "game/game.h"
#include "entities/bullet_pool.h"
#include "entities/enemy.h"
#include "entities/enemy_array.h"
#include "entities/entity.h"
#include "entities/player.h"
#include "game/event_handler.h"
#include "game/game_state.h"
#include "game/main_menu.h"
#include "game/physics.h"
#include "map/map_manager.h"
#include "utils/consts.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void gameLoop(SDL_Renderer *renderer, TextureManager *texManager, Map *map,
              Player *player, EnemyArray *enemies, BulletPool *bulletPool,
              bool *gameRunning, GameState *gameState, Uint32 *last) {
  SDL_Event e;

  while (*gameRunning && *gameState == STATE_GAME) {
    handleGameEvents(&e, player, gameState, gameRunning, bulletPool);

    // Calculate delta time
    Uint32 now = SDL_GetTicks();
    float dt = (now - *last) / 1000.0f;
    *last = now;

    if (player->base.update) {
      player->base.update(&player->base, dt);
    }
    enemyArrayUpdate(enemies, dt, bulletPool);
    bulletPoolUpdate(bulletPool, dt);
    handleCollisions(bulletPool, player, enemies);

    // Check if the player is dead
    if (!isPlayerAlive(player)) {
      printf("Player is dead!\n");
      *gameRunning = false;
      *gameState = STATE_MENU;
    }

    // --- Render ---
    render(renderer, texManager, map, player, bulletPool, enemies);
    SDL_Delay(16);
  }
}

void loadLevel(SDL_Renderer *renderer, TextureManager *texManager,
               GameState *gameState, Map *map) {
  // --- Create/Reset Game Objects ---
  Player *player = playerCreate(320, 400);

  EnemyArray enemies;
  enemyArrayInit(&enemies);

  // Create one enemy per platform using the platform's random position.
  for (size_t i = 0; i < map->platformCount; i++) {
    // Get a random position on the current platform.
    Point enemyPos = getRandomPositionOnPlatform(&map->platforms[i]);
    Enemy *enemy = enemyCreate(enemyPos.x, enemyPos.y);
    enemyArrayAdd(&enemies, enemy);
  }

  BulletPool bulletPool;
  bulletPoolInit(&bulletPool, 100);

  bool gameRunning = true;
  Uint32 last = SDL_GetTicks();

  // Start the game loop
  gameLoop(renderer, texManager, map, player, &enemies, &bulletPool,
           &gameRunning, gameState, &last);

  // --- Clean Up Game Objects ---
  playerDestroy(player);
  enemyArrayDestroy(&enemies);
  bulletPoolDestroy(&bulletPool);
  destroyTextureManager(texManager);
}

void runGame(SDL_Renderer *renderer) {
  GameState gameState = STATE_MENU;

  while (gameState != STATE_EXIT) {
    if (gameState == STATE_MENU) {
      runMainMenu(renderer, &gameState);
      printf("Menu exited with state: %d\n", gameState);
    } else if (gameState == STATE_GAME) {
      const char *maps[] = MAPS;
      size_t numMaps = sizeof(maps) / sizeof(maps[0]);
      MapManager manager;

      initMapManager(&manager, maps, numMaps);
      Map map;
      if (getNextMap(&manager, &map) == 0) {
        printf("Loaded map from: %s\n",
               maps[(manager.currentIndex + manager.mapCount - 1) %
                    manager.mapCount]);
        TextureManager texManager = initTextureManager(
            renderer, PLAYER_TEXTURE_PATH, ENEMY_TEXTURE_PATH,
            BULLET_TEXTURE_PATH, map.backgroundImage, FONT_PATH);
        loadLevel(renderer, &texManager, &gameState, &map);
        mapDestructor(&map);
      } else {
        fprintf(stderr, "Error loading map from: %s\n",
                maps[(manager.currentIndex + manager.mapCount - 1) %
                     manager.mapCount]);
      }
    }
  }
}

