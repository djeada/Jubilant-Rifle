#include "rendering/render.h"
#include "utils/consts.h"
#include <SDL2/SDL_image.h>

// --- Helper: Render an Animated Entity ---
// This cuts out a single frame from the sprite sheet.
static void renderAnimatedEntity(SDL_Renderer *renderer, SDL_Texture *tex,
                                 const SDL_Rect *dest, const Animation *anim,
                                 int frameW, int frameH) {
  SDL_Rect src = {0, 0, frameW, frameH};
  if (anim && anim->frameCount > 0) {
    src.x = anim->currentFrame * frameW;
  }
  SDL_RenderCopy(renderer, tex, &src, dest);
}

void playerDraw(Player *player, SDL_Renderer *renderer, TextureManager *tm) {
  SDL_Rect playerDest = {(int)player->base.pos.x, (int)player->base.pos.y,
                         SPRITE_WIDTH, HUMANOID_FRAME_HEIGHT};
  if (player->base.anim) {
    SDL_Rect src = {player->base.anim->currentFrame * SPRITE_WIDTH, 0,
                    SPRITE_WIDTH, HUMANOID_FRAME_HEIGHT};
    SDL_RenderCopy(renderer, tm->playerTex, &src, &playerDest);
  } else {
    SDL_RenderCopy(renderer, tm->playerTex, NULL, &playerDest);
  }
}

void bulletPoolDraw(BulletPool *pool, SDL_Renderer *renderer,
                    TextureManager *tm) {
  for (int i = 0; i < pool->activeCount; i++) {
    int index = pool->activeIndices[i];
    if (pool->bullets[index] && isEntityAlive(pool->bullets[index])) {
      SDL_Rect dest = {(int)pool->bullets[index]->pos.x,
                       (int)pool->bullets[index]->pos.y, BULLET_SPRITE_WIDTH,
                       BULLET_SPRITE_HEIGHT};
      SDL_RenderCopy(renderer, tm->bulletTex, NULL, &dest);
    }
  }
}

void enemyArrayDraw(EnemyArray *arr, SDL_Renderer *renderer,
                    TextureManager *tm) {
  for (int i = 0; i < arr->count; i++) {
    Entity *enemy = &arr->data[i]->base;
    if (isEntityAlive(enemy)) {
      SDL_Rect dest = {(int)enemy->pos.x, (int)enemy->pos.y, SPRITE_WIDTH,
                       HUMANOID_FRAME_HEIGHT};
      renderAnimatedEntity(renderer, tm->enemyTex, &dest, enemy->anim,
                           SPRITE_WIDTH, HUMANOID_FRAME_HEIGHT);
    }
  }
}

void render(SDL_Renderer *renderer, TextureManager *tm, Player *player,
            BulletPool *pool, EnemyArray *arr) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
  playerDraw(player, renderer, tm);
  enemyArrayDraw(arr, renderer, tm);
  bulletPoolDraw(pool, renderer, tm);
  SDL_RenderPresent(renderer);
}
