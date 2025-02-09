#include "game/game.h"
#include "game/event_handler.h"
#include "game/game_context.h"
#include "game/game_state.h"
#include "game/main_menu.h"
#include "game/physics.h"
#include "map/map_manager.h"
#include "utils/consts.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void gameLoop(GameContext *ctx, bool *gameRunning, GameState *gameState,
              Uint32 *last) {
  SDL_Event e;

  while (*gameRunning && *gameState == STATE_GAME) {
    handleGameEvents(&e, ctx->player, gameState, gameRunning, ctx->bulletPool);

    // Calculate delta time.
    Uint32 now = SDL_GetTicks();
    float dt = (now - *last) / 1000.0f;
    *last = now;

    if (ctx->player->base.update) {
      ctx->player->base.update(&ctx->player->base, dt);
    }
    enemyArrayUpdate(ctx->enemies, dt, ctx->bulletPool);
    bulletPoolUpdate(ctx->bulletPool, dt);
    handleCollisions(ctx->bulletPool, ctx->player, ctx->enemies);

    if (!isPlayerAlive(ctx->player)) {
      printf("Player is dead!\n");
      *gameRunning = false;
      *gameState = STATE_MENU;
    }

    // Render everything.
    renderGame(ctx);
    SDL_Delay(16);
  }
}

void loadLevel(SDL_Renderer *renderer, TextureManager *texManager,
               GameState *gameState, Map *map) {
  // Create game objects dynamically.
  Player *player = playerCreate(320, 400);

  // Allocate and initialize the enemy array on the heap.
  EnemyArray *enemies = malloc(sizeof(EnemyArray));
  enemyArrayInit(enemies);

  // Create one enemy per platform.
  for (size_t i = 0; i < map->platformCount; i++) {
    Point enemyPos = getRandomPositionOnPlatform(&map->platforms[i]);
    Enemy *enemy = enemyCreate(enemyPos.x, enemyPos.y);
    enemyArrayAdd(enemies, enemy);
  }

  // Allocate and initialize the bullet pool on the heap.
  BulletPool *bulletPool = malloc(sizeof(BulletPool));
  bulletPoolInit(bulletPool, 100);

  bool gameRunning = true;
  Uint32 last = SDL_GetTicks();

  // Create the game context struct with heap-allocated objects.
  GameContext ctx = {
      .renderer = renderer,
      .texManager = texManager,
      .map = map,
      .player = player,
      .enemies = enemies,
      .bulletPool = bulletPool,
  };

  // Start the game loop.
  gameLoop(&ctx, &gameRunning, gameState, &last);

  // --- Clean Up Game Objects ---
  playerDestroy(player);
  enemyArrayDestroy(enemies);
  bulletPoolDestroy(bulletPool);
  destroyTextureManager(texManager);

  // Free the heap-allocated memory.
  free(enemies);
  free(bulletPool);
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

