#include "rendering/render.h"

// A helper to create a solid-colored texture.
static SDL_Texture* createFallbackTexture(SDL_Renderer *renderer, SDL_Color color, int w, int h) {
    SDL_Texture *tex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
    SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(renderer, tex);
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(renderer);
    SDL_SetRenderTarget(renderer, NULL);
    return tex;
}

TextureManager initTextureManager(SDL_Renderer *renderer) {
    TextureManager tm;
    tm.playerTex = createFallbackTexture(renderer, (SDL_Color){0, 0, 255, 255}, 50, 50);
    tm.enemyTex  = createFallbackTexture(renderer, (SDL_Color){0, 255, 0, 255}, 50, 50);
    tm.bulletTex = createFallbackTexture(renderer, (SDL_Color){255, 0, 0, 255}, 10, 10);
    return tm;
}

void destroyTextureManager(TextureManager *tm) {
    SDL_DestroyTexture(tm->playerTex);
    SDL_DestroyTexture(tm->enemyTex);
    SDL_DestroyTexture(tm->bulletTex);
}

void bullet_pool_draw(BulletPool *pool, SDL_Renderer *renderer, TextureManager *tm) {
    for (int i = 0; i < BULLET_POOL_CAPACITY; i++) {
        if (pool->bullets[i] && pool->bullets[i]->alive) {
            SDL_Rect r = { (int)pool->bullets[i]->pos.x, (int)pool->bullets[i]->pos.y, 10, 10 };
            SDL_RenderCopy(renderer, tm->bulletTex, NULL, &r);
        }
    }
}

void enemy_array_draw(EnemyArray *arr, SDL_Renderer *renderer, TextureManager *tm) {
    for (int i = 0; i < arr->count; i++) {
        if (arr->data[i]->base.alive) {
            SDL_Rect r = { (int)arr->data[i]->base.pos.x, (int)arr->data[i]->base.pos.y, 50, 50 };
            SDL_RenderCopy(renderer, tm->enemyTex, NULL, &r);
        }
    }
}
