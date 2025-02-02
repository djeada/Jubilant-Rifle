#ifndef RENDER_H
#define RENDER_H

#include <SDL2/SDL.h>
#include "rendering/texture_manager.h"
#include "entities/bullet_pool.h"
#include "entities/enemy_array.h"
#include "entities/player.h"
#include "map/map.h"

void render(SDL_Renderer *renderer, TextureManager *tm, Map *map, Player *player,
            BulletPool *pool, EnemyArray *arr);
            

#endif // RENDER_H
