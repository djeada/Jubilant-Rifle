#ifndef RENDER_H
#define RENDER_H

#include <SDL2/SDL.h>
#include "entities/bullet_pool.h"
#include "entities/enemy_array.h"

// A simple texture manager structure to hold our game textures.
typedef struct {
    SDL_Texture *playerTex;
    SDL_Texture *enemyTex;
    SDL_Texture *bulletTex;
} TextureManager;

// Initializes textures using fallback colors.
TextureManager initTextureManager(SDL_Renderer *renderer);

// Frees all textures.
void destroyTextureManager(TextureManager *tm);

void render(SDL_Renderer *renderer, TextureManager *tm, Entity* player, BulletPool *pool, EnemyArray *arr);
void player_draw(Entity* player, SDL_Renderer *renderer, TextureManager *tm) ;
void bullet_pool_draw(BulletPool *pool, SDL_Renderer *renderer, TextureManager *tm);
void enemy_array_draw(EnemyArray *arr, SDL_Renderer *renderer, TextureManager *tm);

#endif // RENDER_H
