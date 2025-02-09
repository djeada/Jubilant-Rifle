#include "rendering/render.h"
#include "map/map.h" // For the Map definition.
#include "utils/consts.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

// ---------------------------------------------------------------------------
// Utility inline functions
// ---------------------------------------------------------------------------

static inline int isRectVisible(const SDL_Rect *entityRect,
                                const SDL_Rect *camera) {
  return SDL_HasIntersection(entityRect, camera);
}

static inline void applyCamera(SDL_Rect *dest, const SDL_Rect *camera) {
  dest->x -= camera->x;
  dest->y -= camera->y;
}

// ---------------------------------------------------------------------------
// Entity Rendering Helpers
// ---------------------------------------------------------------------------

static void renderAnimatedEntity(SDL_Renderer *renderer, SDL_Texture *tex,
                                 const SDL_Rect *dest, const Animation *anim,
                                 int frameW, int frameH) {
  SDL_Rect src = {0, 0, frameW, frameH};
  if (anim && anim->frameCount > 0) {
    src.x = anim->currentFrame * frameW;
  }
  SDL_RenderCopy(renderer, tex, &src, dest);
}

// ---------------------------------------------------------------------------
// Entity Draw Functions
// ---------------------------------------------------------------------------

void playerDraw(Player *player, SDL_Renderer *renderer, TextureManager *tm,
                const SDL_Rect *camera) {
  SDL_Rect playerRect = {(int)player->base.pos.x, (int)player->base.pos.y,
                         SPRITE_WIDTH, HUMANOID_FRAME_HEIGHT};

  if (!isRectVisible(&playerRect, camera))
    return;

  SDL_Rect playerDest = playerRect;
  applyCamera(&playerDest, camera);

  if (player->base.anim) {
    SDL_Rect src = {player->base.anim->currentFrame * SPRITE_WIDTH, 0,
                    SPRITE_WIDTH, HUMANOID_FRAME_HEIGHT};
    SDL_RenderCopy(renderer, tm->playerTex, &src, &playerDest);
  } else {
    SDL_RenderCopy(renderer, tm->playerTex, NULL, &playerDest);
  }
}

void bulletPoolDraw(BulletPool *pool, SDL_Renderer *renderer,
                    TextureManager *tm, const SDL_Rect *camera) {
  // --- Draw Player Bullets ---
  for (int i = 0; i < pool->playerActiveCount; i++) {
    int index = pool->playerActiveIndices[i];
    Bullet *bullet = pool->playerBullets[index];
    if (bullet && isBulletAlive(bullet)) {
      SDL_Rect bulletRect = {(int)bullet->base.pos.x, (int)bullet->base.pos.y,
                             BULLET_SPRITE_WIDTH, BULLET_SPRITE_HEIGHT};
      if (!isRectVisible(&bulletRect, camera))
        continue;

      SDL_Rect dest = bulletRect;
      applyCamera(&dest, camera);
      SDL_RenderCopy(renderer, tm->bulletTex, NULL, &dest);
    }
  }

  // --- Draw Enemy Bullets ---
  for (int i = 0; i < pool->enemyActiveCount; i++) {
    int index = pool->enemyActiveIndices[i];
    Bullet *bullet = pool->enemyBullets[index];
    if (bullet && isBulletAlive(bullet)) {
      SDL_Rect bulletRect = {(int)bullet->base.pos.x, (int)bullet->base.pos.y,
                             BULLET_SPRITE_WIDTH, BULLET_SPRITE_HEIGHT};
      if (!isRectVisible(&bulletRect, camera))
        continue;

      SDL_Rect dest = bulletRect;
      applyCamera(&dest, camera);
      SDL_RenderCopy(renderer, tm->bulletTex, NULL, &dest);
    }
  }
}

void enemyArrayDraw(EnemyArray *arr, SDL_Renderer *renderer, TextureManager *tm,
                    const SDL_Rect *camera) {
  for (int i = 0; i < arr->count; i++) {
    Entity *enemy = &arr->data[i]->base;
    if (isEntityAlive(enemy)) {
      SDL_Rect enemyRect = {(int)enemy->pos.x, (int)enemy->pos.y, SPRITE_WIDTH,
                            HUMANOID_FRAME_HEIGHT};
      if (!isRectVisible(&enemyRect, camera))
        continue;

      SDL_Rect dest = enemyRect;
      applyCamera(&dest, camera);
      renderAnimatedEntity(renderer, tm->enemyTex, &dest, enemy->anim,
                           SPRITE_WIDTH, HUMANOID_FRAME_HEIGHT);
    }
  }
}

// ---------------------------------------------------------------------------
// Platform Rendering
// ---------------------------------------------------------------------------

void renderMapPlatforms(SDL_Renderer *renderer, Map *map,
                        const SDL_Rect *camera) {
  // Set a draw color for the platforms (e.g., grey).
  SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
  for (size_t i = 0; i < map->platformCount; i++) {
    SDL_Rect platformRect = map->platforms[i];
    // Check if the platform is visible in the camera view.
    if (!isRectVisible(&platformRect, camera))
      continue;
    // Convert the platform rectangle from world space to screen space.
    applyCamera(&platformRect, camera);
    SDL_RenderFillRect(renderer, &platformRect);
  }
}

// ---------------------------------------------------------------------------
// UI Rendering Functions (remain in screen-space)
// ---------------------------------------------------------------------------

static void renderText(SDL_Renderer *renderer, TTF_Font *font, const char *text,
                       int x, int y, SDL_Color color) {
  SDL_Surface *surface = TTF_RenderText_Solid(font, text, color);
  if (!surface) {
    SDL_Log("Unable to render text surface: %s", TTF_GetError());
    return;
  }
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
  if (!texture) {
    SDL_Log("Unable to create texture from rendered text: %s", SDL_GetError());
    SDL_FreeSurface(surface);
    return;
  }
  SDL_Rect dstRect = {x, y, surface->w, surface->h};
  SDL_RenderCopy(renderer, texture, NULL, &dstRect);
  SDL_DestroyTexture(texture);
  SDL_FreeSurface(surface);
}

static void renderHealthBar(SDL_Renderer *renderer, Player *player,
                            TTF_Font *font) {
  const int bgWidth = HEALTH_BAR_BG_WIDTH * HEALTH_BAR_SCALE;
  const int bgHeight = HEALTH_BAR_BG_HEIGHT * HEALTH_BAR_SCALE;
  const int barWidth = HEALTH_BAR_WIDTH * HEALTH_BAR_SCALE;
  const int barHeight = HEALTH_BAR_HEIGHT * HEALTH_BAR_SCALE;

  SDL_Rect bgRect = {HEALTH_BAR_OFFSET_X, HEALTH_BAR_OFFSET_Y, bgWidth,
                     bgHeight};

  SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
  SDL_RenderFillRect(renderer, &bgRect);

  float healthRatio =
      (player->base.health > 0) ? (float)player->base.health / 100 : 0;
  int filledBarWidth = (int)(barWidth * healthRatio);

  SDL_Rect healthRect = {bgRect.x + (bgWidth - barWidth) / 2,
                         bgRect.y + (bgHeight - barHeight) / 2, filledBarWidth,
                         barHeight};

  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  SDL_RenderFillRect(renderer, &healthRect);

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderDrawRect(renderer, &bgRect);

  char levelText[32];
  snprintf(levelText, sizeof(levelText), "%s%d", HEALTH_BAR_TEXT,
           player->level);
  SDL_Color textColor = {255, 255, 255, 255};
  int textX = HEALTH_BAR_OFFSET_X;
  int textY = HEALTH_BAR_OFFSET_Y + bgHeight + 5;
  renderText(renderer, font, levelText, textX, textY, textColor);
}

// ---------------------------------------------------------------------------
// Main Render Function with Camera Support and Entity Filtering
// ---------------------------------------------------------------------------

void renderGame(GameContext *ctx) {
  // Compute the camera view (world coordinates) centered on the player.
  SDL_Rect camera;
  camera.w = GAME_WIDTH;
  camera.h = GAME_HEIGHT;
  camera.x = (int)(ctx->player->base.pos.x + SPRITE_WIDTH / 2 - GAME_WIDTH / 2);
  camera.y = (int)(ctx->player->base.pos.y + HUMANOID_FRAME_HEIGHT / 2 -
                   GAME_HEIGHT / 2);

  // Clamp the camera to the map bounds.
  if (camera.x < ctx->map->rect.x) {
    camera.x = ctx->map->rect.x;
  }
  if (camera.y < ctx->map->rect.y) {
    camera.y = ctx->map->rect.y;
  }
  if (camera.x > ctx->map->rect.x + ctx->map->rect.w - camera.w) {
    camera.x = ctx->map->rect.x + ctx->map->rect.w - camera.w;
  }
  if (camera.y > ctx->map->rect.y + ctx->map->rect.h - camera.h) {
    camera.y = ctx->map->rect.y + ctx->map->rect.h - camera.h;
  }

  // Clear the renderer.
  SDL_SetRenderDrawColor(ctx->renderer, 0, 0, 0, 255);
  SDL_RenderClear(ctx->renderer);

  // Render the background texture.
  SDL_Rect bgSrc = camera;
  SDL_Rect bgDest = {0, 0, GAME_WIDTH, GAME_HEIGHT};
  SDL_RenderCopy(ctx->renderer, ctx->texManager->backgroundTex, &bgSrc,
                 &bgDest);

  // Render game entities relative to the camera.
  playerDraw(ctx->player, ctx->renderer, ctx->texManager, &camera);
  enemyArrayDraw(ctx->enemies, ctx->renderer, ctx->texManager, &camera);
  bulletPoolDraw(ctx->bulletPool, ctx->renderer, ctx->texManager, &camera);

  // Render the map platforms.
  renderMapPlatforms(ctx->renderer, ctx->map, &camera);

  // Render the UI (e.g., health bar, level text).
  renderHealthBar(ctx->renderer, ctx->player, ctx->texManager->font);

  // Present the final rendered frame.
  SDL_RenderPresent(ctx->renderer);
}

