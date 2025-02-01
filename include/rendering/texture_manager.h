#ifndef TEXT_MANAGER_H
#define TEXT_MANAGER_H

#include <SDL2/SDL.h>
#include "entities/bullet_pool.h"
#include "entities/enemy_array.h"  

typedef struct {
    SDL_Texture *playerTex;
    SDL_Texture *enemyTex;
    SDL_Texture *bulletTex;
} TextureManager;

TextureManager initTextureManager(SDL_Renderer *renderer);
void destroyTextureManager(TextureManager *tm);

#endif // TEXT_MANAGER_H
