#include "rendering/render.h"
#include "utils/consts.h"
#include <SDL2/SDL_image.h>
#include <stdio.h>

// --- Helper: Create Fallback Texture --- 
static SDL_Texture* createFallbackTexture(SDL_Renderer *renderer, SDL_Color color, int w, int h) {
    SDL_Texture *tex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                                          SDL_TEXTUREACCESS_TARGET, w, h);
    if (!tex) {
        fprintf(stderr, "Failed to create fallback texture: %s\n", SDL_GetError());
        return NULL;
    }
    SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(renderer, tex);
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(renderer);
    SDL_SetRenderTarget(renderer, NULL);
    return tex;
}

// --- Helper: Load Texture From File or Fallback ---
static SDL_Texture* loadTextureOrFallback(SDL_Renderer *renderer, const char *path,
                                            SDL_Color fallbackColor, int fallbackW, int fallbackH) {
    SDL_Texture *texture = IMG_LoadTexture(renderer, path);
    if (!texture) {
        fprintf(stderr, "Failed to load texture from %s: %s\n", path, IMG_GetError());
        texture = createFallbackTexture(renderer, fallbackColor, fallbackW, fallbackH);
    }
    return texture;
}

// --- Public: Initialize Texture Manager ---
TextureManager initTextureManager(SDL_Renderer *renderer) {
    TextureManager tm;
    tm.playerTex = loadTextureOrFallback(renderer, PLAYER_TEXTURE_PATH,
                                           (SDL_Color){0, 0, 255, 255}, SPRITE_WIDTH, SPRITE_HEIGHT);
    tm.enemyTex  = loadTextureOrFallback(renderer, ENEMY_TEXTURE_PATH,
                                           (SDL_Color){0, 255, 0, 255}, SPRITE_WIDTH, SPRITE_HEIGHT);
    tm.bulletTex = loadTextureOrFallback(renderer, BULLET_TEXTURE_PATH,
                                           (SDL_Color){255, 0, 0, 255}, BULLET_SPRITE_WIDTH, BULLET_SPRITE_HEIGHT);
    return tm;
}

// --- Public: Destroy Texture Manager ---
void destroyTextureManager(TextureManager *tm) {
    SDL_DestroyTexture(tm->playerTex);
    SDL_DestroyTexture(tm->enemyTex);
    SDL_DestroyTexture(tm->bulletTex);
}

// --- Helper: Render an Entity with Animation ---
// Renders only the appropriate frame from the texture based on the entity's Animation data.
// If no animation is set, the full texture is rendered.
static void renderAnimatedEntity(SDL_Renderer *renderer, SDL_Texture *tex, 
                                 const SDL_Rect *dest, const Animation *anim,
                                 int frameW, int frameH) {
    SDL_Rect src = {0, 0, frameW, frameH};
    if (anim && anim->frameCount > 0) {
        src.x = anim->currentFrame * frameW;
    }
    SDL_RenderCopy(renderer, tex, &src, dest);
}

// --- Public: Draw Bullet Pool ---
// Bullets typically do not use animation, so we simply render the entire texture.
void bullet_pool_draw(BulletPool *pool, SDL_Renderer *renderer, TextureManager *tm) {
    for (int i = 0; i < BULLET_POOL_CAPACITY; i++) {
        if (pool->bullets[i] && pool->bullets[i]->alive) {
            SDL_Rect dest = { (int)pool->bullets[i]->pos.x,
                              (int)pool->bullets[i]->pos.y,
                              BULLET_SPRITE_WIDTH, BULLET_SPRITE_HEIGHT };
            SDL_RenderCopy(renderer, tm->bulletTex, NULL, &dest);
        }
    }
}

// --- Public: Draw Enemy Array ---
// Uses the enemy's animation state to render the correct frame.
void enemy_array_draw(EnemyArray *arr, SDL_Renderer *renderer, TextureManager *tm) {
    for (int i = 0; i < arr->count; i++) {
        Entity *enemy = &arr->data[i]->base;
        if (enemy->alive) {
            SDL_Rect dest = { (int)enemy->pos.x, (int)enemy->pos.y, SPRITE_WIDTH, SPRITE_HEIGHT };
            renderAnimatedEntity(renderer, tm->enemyTex, &dest, enemy->anim, SPRITE_WIDTH, SPRITE_HEIGHT);
        }
    }
}
