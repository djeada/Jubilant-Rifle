#include "game/game.h"
#include "game/event_handler.h"

#include "entities/entity.h"
#include "entities/bullet_pool.h"
#include "entities/enemy.h"
#include "entities/player.h"
#include "entities/enemy_array.h"
#include "game/game_state.h"
#include "game/main_menu.h"

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

void runGame(SDL_Renderer *renderer, TextureManager *texManager) {
    GameState gameState = STATE_MENU;
    while (gameState != STATE_EXIT) {
        if (gameState == STATE_MENU) {
            runMainMenu(renderer, &gameState);
        }
        else if (gameState == STATE_GAME) {
            // --- Create/Reset Game Objects ---
            Entity *player = entity_create(ENTITY_PLAYER, 320, 400);
            player->update = player_update;
            
            EnemyArray enemies;
            enemy_array_init(&enemies);
            for (int i = 0; i < 5; i++) {
                Enemy *enemy = malloc(sizeof(Enemy));
                enemy->base = *entity_create(ENTITY_ENEMY, 50 + i * 100, 50);
                enemy->base.update = enemy_update;
                enemy->shootTimer = 2.0f;
                enemy_array_add(&enemies, enemy);
            }
            
            BulletPool bulletPool;
            bullet_pool_init(&bulletPool);

            bool gameRunning = true;
            Uint32 last = SDL_GetTicks();
            SDL_Event e;
            while (gameRunning && gameState == STATE_GAME) {
                // --- Event Handling (delegated to event_handler.c) ---
                handleGameEvents(&e, player, &gameState, &gameRunning);
                
                // --- Update ---
                Uint32 now = SDL_GetTicks();
                float dt = (now - last) / 1000.0f;
                last = now;
                
                if (player->update)
                    player->update(player, dt);
                enemy_array_update(&enemies, dt, &bulletPool);
                bullet_pool_update(&bulletPool, dt);
                
                // --- Render (delegated to render.c) ---
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);
                
                SDL_Rect pr = { (int)player->pos.x, (int)player->pos.y, 50, 50 };
                SDL_RenderCopy(renderer, texManager->playerTex, NULL, &pr);
                enemy_array_draw(&enemies, renderer, texManager);
                bullet_pool_draw(&bulletPool, renderer, texManager);
                
                SDL_RenderPresent(renderer);
                SDL_Delay(10);
            }
            
            // --- Clean Up Game Objects ---
            entity_destroy(player);
            enemy_array_destroy(&enemies);
            bullet_pool_destroy(&bulletPool);
        }
    }
}
