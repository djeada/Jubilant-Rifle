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
#include "utils/consts.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void runGame(SDL_Renderer *renderer, TextureManager *texManager) {
  GameState gameState = STATE_MENU;
  while (gameState != STATE_EXIT) {
    if (gameState == STATE_MENU) {
      runMainMenu(renderer, &gameState);
      printf("Menu exited with state: %d\n", gameState);
    } else if (gameState == STATE_GAME || true) {
      // --- Create/Reset Game Objects ---
      Entity *player = playerCreate(320, 400);

      EnemyArray enemies;
      enemyArrayInit(&enemies);
      for (int i = 0; i < 5; i++) {
        Enemy *enemy = enemyCreate(50 + i * 100, 50);
        enemyArrayAdd(&enemies, enemy);
      }

      BulletPool bulletPool;
      bulletPoolInit(&bulletPool);

      bool gameRunning = true;
      Uint32 last = SDL_GetTicks();
      SDL_Event e;
      while (gameRunning && gameState == STATE_GAME) {
        handleGameEvents(&e, player, &gameState, &gameRunning, &bulletPool);

        Uint32 now = SDL_GetTicks();
        float dt = (now - last) / 1000.0f;
        last = now;

        if (player->update)
          player->update(player, dt);
        enemyArrayUpdate(&enemies, dt, &bulletPool);
        bulletPoolUpdate(&bulletPool, dt);
        handleCollisions(&bulletPool, player, &enemies);

        if (player->health <= 0) {
          printf("Player is dead!\n");
          gameRunning = false;
          gameState = STATE_MENU;
        }

        // --- Render ---
        render(renderer, texManager, player, &bulletPool, &enemies);
        SDL_Delay(10);
      }

      // --- Clean Up Game Objects ---
      entityDestroy(player);
      enemyArrayDestroy(&enemies);
      bulletPoolDestroy(&bulletPool);
    }
  }
}
