#include "game/game.h"
#include "game/event_handler.h"

#include "entities/entity.h"
#include "entities/enemy.h"
#include "entities/player.h"
#include "entities/enemy_array.h"
#include "entities/bullet_pool.h"
#include "game/game_state.h"
#include "game/main_menu.h"
#include "utils/consts.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

// Helper: check collisions between bullets and entities.
static void handleCollisions(BulletPool *pool, Entity *player, EnemyArray *enemies) {
    for (int i = 0; i < BULLET_POOL_CAPACITY; i++) {
        Entity *bullet = pool->bullets[i];
        if (!bullet || !bullet->alive)
            continue;
            
        SDL_Rect bulletRect = { (int)bullet->pos.x, (int)bullet->pos.y, 10, 10 };

        // Use bullet's vertical velocity to determine its origin.
        if (bullet->vel.y > 0) {  // enemy bullet
            SDL_Rect playerRect = { (int)player->pos.x, (int)player->pos.y, 50, 50 };
            if (SDL_HasIntersection(&bulletRect, &playerRect)) {
                player->health -= 10;  // Adjust damage as needed.
                bullet->alive = 0;
            }
        }
        else if (bullet->vel.y < 0) {  // player bullet
            for (int j = 0; j < enemies->count; j++) {
                Entity *enemyEntity = (Entity *)&enemies->data[j]->base;
                if (!enemyEntity->alive)
                    continue;
                SDL_Rect enemyRect = { (int)enemyEntity->pos.x, (int)enemyEntity->pos.y, 50, 50 };
                if (SDL_HasIntersection(&bulletRect, &enemyRect)) {
                    enemyEntity->health -= 10;
                    bullet->alive = 0;
                    if (enemyEntity->health <= 0)
                        enemyEntity->alive = 0;
                }
            }
        }
    }
}

void runGame(SDL_Renderer *renderer, TextureManager *texManager) {
    GameState gameState = STATE_MENU;
    while (gameState != STATE_EXIT) {
        if (gameState == STATE_MENU) {
            runMainMenu(renderer, &gameState);
        }
        else if (gameState == STATE_GAME) {
            // --- Create/Reset Game Objects ---
            Entity *player = entity_create(ENTITY_PLAYER, 320, 400);
            player->health = 100;
            player->update = player_update;
            // Initialize player animation:
            player->anim = malloc(sizeof(Animation));
            player->anim->frameCount = 6;         // e.g. 6 frames in the sprite sheet
            player->anim->frameDuration = 0.1f;     // each frame lasts 0.1 seconds
            player->anim->timer = 0;
            player->anim->currentFrame = 0;
            
            EnemyArray enemies;
            enemy_array_init(&enemies);
            for (int i = 0; i < 5; i++) {
                Enemy *enemy = malloc(sizeof(Enemy));
                enemy->base = *entity_create(ENTITY_ENEMY, 50 + i * 100, 50);
                enemy->base.health = 30;
                enemy->base.update = enemy_update;
                enemy->shootTimer = 2.0f;
                // Initialize enemy animation:
                enemy->base.anim = malloc(sizeof(Animation));
                enemy->base.anim->frameCount = 6;     // adjust if needed
                enemy->base.anim->frameDuration = 0.2f; // slower animation, for example
                enemy->base.anim->timer = 0;
                enemy->base.anim->currentFrame = 0;
                enemy_array_add(&enemies, enemy);
            }
            
            BulletPool bulletPool;
            bullet_pool_init(&bulletPool);
            
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
                enemy_array_update(&enemies, dt, &bulletPool);
                bullet_pool_update(&bulletPool, dt);
                handleCollisions(&bulletPool, player, &enemies);
                
                if (player->health <= 0) {
                    printf("Player is dead!\n");
                    gameRunning = false;
                    gameState = STATE_MENU;
                }
                
                // --- Render ---
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);
                
                // Render player using animation state.
                SDL_Rect playerDest = { (int)player->pos.x, (int)player->pos.y, SPRITE_WIDTH, HUMANOID_FRAME_HEIGHT };
                if (player->anim) {
                    SDL_Rect src = { player->anim->currentFrame * SPRITE_WIDTH, 0, SPRITE_WIDTH, HUMANOID_FRAME_HEIGHT };
                    SDL_RenderCopy(renderer, texManager->playerTex, &src, &playerDest);
                } else {
                    SDL_RenderCopy(renderer, texManager->playerTex, NULL, &playerDest);
                }
                
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
