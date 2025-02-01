#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "entities/entity.h"
#include "entities/bullet_pool.h"
#include "entities/enemy.h"
#include "entities/player.h"

/* --- Texture Manager --- */
typedef struct {
    SDL_Texture *playerTex;
    SDL_Texture *enemyTex;
    SDL_Texture *bulletTex;
} TextureManager;

TextureManager texManager;
void bullet_pool_draw(BulletPool *pool, SDL_Renderer *renderer) {
    for (int i = 0; i < BULLET_POOL_CAPACITY; i++) {
        if (pool->bullets[i] && pool->bullets[i]->alive) {
            SDL_Rect r = { (int)pool->bullets[i]->pos.x, (int)pool->bullets[i]->pos.y, 10, 10 };
            SDL_RenderCopy(renderer, texManager.bulletTex, NULL, &r);
        }
    }
}



SDL_Texture* createFallbackTexture(SDL_Renderer *renderer, SDL_Color color, int w, int h) {
    SDL_Texture *tex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
    SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(renderer, tex);
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(renderer);
    SDL_SetRenderTarget(renderer, NULL);
    return tex;
}



/* --- Enemy Container (Dynamic Array) --- */
typedef struct {
    Enemy **data;
    int count;
    int capacity;
} EnemyArray;

void enemy_array_init(EnemyArray *arr) {
    arr->capacity = 10;
    arr->count = 0;
    arr->data = malloc(sizeof(Enemy*) * arr->capacity);
}

void enemy_array_add(EnemyArray *arr, Enemy *enemy) {
    if (arr->count >= arr->capacity) {
        arr->capacity *= 2;
        arr->data = realloc(arr->data, sizeof(Enemy*) * arr->capacity);
    }
    arr->data[arr->count++] = enemy;
}

void enemy_array_update(EnemyArray *arr, float dt, BulletPool *pool) {
    for (int i = 0; i < arr->count; i++) {
        Enemy *enemy = arr->data[i];
        if (enemy->base.alive) {
            enemy->base.update((Entity*)enemy, dt);
            if (enemy->shootTimer <= 0) {
                bullet_pool_spawn(pool, enemy->base.pos.x + 20, enemy->base.pos.y + 50, 0, 150);
                enemy->shootTimer = 2.0f;
            }
        }
    }
}
void enemy_array_destroy(EnemyArray *arr) {
    for (int i = 0; i < arr->count; i++) {
        if (arr->data[i])
            entity_destroy((Entity*)arr->data[i]);
    }
    free(arr->data);
}
void enemy_array_draw(EnemyArray *arr, SDL_Renderer *renderer) {
    for (int i = 0; i < arr->count; i++) {
        if (arr->data[i]->base.alive) {
            SDL_Rect r = { (int)arr->data[i]->base.pos.x, (int)arr->data[i]->base.pos.y, 50, 50 };
            SDL_RenderCopy(renderer, texManager.enemyTex, NULL, &r);
        }
    }
}



typedef enum {
    STATE_MENU,
    STATE_GAME,
    STATE_EXIT
} GameState;

/* --- Insert this function before main() --- */
void runMainMenu(SDL_Renderer *renderer, GameState *state) {
    SDL_Event event;
    bool menuRunning = true;
    while (menuRunning) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                *state = STATE_EXIT;
                return;
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_RETURN) {
                    *state = STATE_GAME;
                    return;
                }
            }
        }
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
        SDL_RenderClear(renderer);
        /* Optionally render menu text/buttons here */
        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }
}


/* --- Main --- */
int main(int argc, char *argv[]) {
    (void)argc; (void)argv;
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }
    SDL_Window *win = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, 0);
    if (!win) {
        SDL_Quit();
        return 1;
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    /* --- Texture Manager Initialization --- */
    texManager.playerTex = createFallbackTexture(renderer, (SDL_Color){0, 0, 255, 255}, 50, 50);
    texManager.enemyTex  = createFallbackTexture(renderer, (SDL_Color){0, 255, 0, 255}, 50, 50);
    texManager.bulletTex = createFallbackTexture(renderer, (SDL_Color){255, 0, 0, 255}, 10, 10);

    GameState gameState = STATE_MENU;

    while (gameState != STATE_EXIT) {
        if (gameState == STATE_MENU) {
            runMainMenu(renderer, &gameState);
        }
        else if (gameState == STATE_GAME) {
            /* --- Create/Reset Game Objects --- */
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
                while (SDL_PollEvent(&e)) {
                    if (e.type == SDL_QUIT) {
                        gameState = STATE_EXIT;
                        gameRunning = false;
                    }
                    if (e.type == SDL_KEYDOWN) {
                        if (e.key.keysym.sym == SDLK_ESCAPE) {
                            gameState = STATE_MENU;
                            gameRunning = false;
                        }
                        if (e.key.keysym.sym == SDLK_LEFT)
                            player->vel.x = -200;
                        if (e.key.keysym.sym == SDLK_RIGHT)
                            player->vel.x = 200;
                        if (e.key.keysym.sym == SDLK_UP)
                            player->vel.y = -200;
                        if (e.key.keysym.sym == SDLK_DOWN)
                            player->vel.y = 200;
                    }
                    if (e.type == SDL_KEYUP) {
                        if (e.key.keysym.sym == SDLK_LEFT || e.key.keysym.sym == SDLK_RIGHT)
                            player->vel.x = 0;
                        if (e.key.keysym.sym == SDLK_UP || e.key.keysym.sym == SDLK_DOWN)
                            player->vel.y = 0;
                    }
                }

                Uint32 now = SDL_GetTicks();
                float dt = (now - last) / 1000.0f;
                last = now;

                if (player->update)
                    player->update(player, dt);
                enemy_array_update(&enemies, dt, &bulletPool);
                bullet_pool_update(&bulletPool, dt);

                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);

                SDL_Rect pr = { (int)player->pos.x, (int)player->pos.y, 50, 50 };
                SDL_RenderCopy(renderer, texManager.playerTex, NULL, &pr);
                enemy_array_draw(&enemies, renderer);
                bullet_pool_draw(&bulletPool, renderer);

                SDL_RenderPresent(renderer);
                SDL_Delay(10);
            }

            /* --- Clean Up Game Objects --- */
            entity_destroy(player);
            enemy_array_destroy(&enemies);
            bullet_pool_destroy(&bulletPool);
        }
    }

    SDL_DestroyTexture(texManager.playerTex);
    SDL_DestroyTexture(texManager.enemyTex);
    SDL_DestroyTexture(texManager.bulletTex);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}


