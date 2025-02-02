#ifndef RENDER_H
#define RENDER_H

#include <SDL2/SDL.h>
#include "rendering/texture_manager.h"
#include "entities/player.h"

void render(SDL_Renderer *renderer, TextureManager *tm, Player* player, BulletPool *pool, EnemyArray *arr);
void playerDraw(Player* player, SDL_Renderer *renderer, TextureManager *tm) ;
void bulletPoolDraw(BulletPool *pool, SDL_Renderer *renderer, TextureManager *tm);
void enemyArrayDraw(EnemyArray *arr, SDL_Renderer *renderer, TextureManager *tm);

#endif // RENDER_H
