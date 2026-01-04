#include "rendering/render.h"
#include "map/map.h" // For the Map definition.
#include "map/ladder.h"
#include "map/trap.h"
#include "map/flag.h"
#include "entities/grenade.h"
#include "entities/grenade_pool.h"
#include "game/score.h"
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
// Level Timer Rendering
// ---------------------------------------------------------------------------

static void renderTimer(SDL_Renderer *renderer, TTF_Font *font, 
                        const ScoreTracker *tracker) {
  if (!tracker)
    return;
    
  char timeStr[16];
  scoreGetElapsedTimeString(tracker, timeStr, sizeof(timeStr));
  
  SDL_Color textColor = {255, 255, 255, 255};
  renderText(renderer, font, timeStr, TIMER_DISPLAY_X, TIMER_DISPLAY_Y, textColor);
}

// ---------------------------------------------------------------------------
// Ladder Rendering
// ---------------------------------------------------------------------------

static void renderLadders(SDL_Renderer *renderer, const LadderArray *ladders,
                           const SDL_Rect *camera) {
  if (!ladders)
    return;
    
  /* Set color for ladders (brown/wood color) */
  SDL_SetRenderDrawColor(renderer, 139, 90, 43, 255);
  
  for (size_t i = 0; i < ladders->count; i++) {
    SDL_Rect ladderRect = ladders->ladders[i].rect;
    
    if (!isRectVisible(&ladderRect, camera))
      continue;
      
    applyCamera(&ladderRect, camera);
    
    /* Draw ladder sides */
    SDL_Rect leftSide = {ladderRect.x, ladderRect.y, 5, ladderRect.h};
    SDL_Rect rightSide = {ladderRect.x + ladderRect.w - 5, ladderRect.y, 5, ladderRect.h};
    SDL_RenderFillRect(renderer, &leftSide);
    SDL_RenderFillRect(renderer, &rightSide);
    
    /* Draw rungs */
    int rungSpacing = 20;
    for (int y = ladderRect.y + 10; y < ladderRect.y + ladderRect.h - 10; y += rungSpacing) {
      SDL_Rect rung = {ladderRect.x + 5, y, ladderRect.w - 10, 4};
      SDL_RenderFillRect(renderer, &rung);
    }
  }
}

// ---------------------------------------------------------------------------
// Trap Rendering
// ---------------------------------------------------------------------------

static void renderTraps(SDL_Renderer *renderer, const TrapArray *traps,
                         const SDL_Rect *camera) {
  if (!traps)
    return;
    
  for (size_t i = 0; i < traps->count; i++) {
    const Trap *trap = &traps->traps[i];
    SDL_Rect trapRect = trap->rect;
    
    if (!isRectVisible(&trapRect, camera))
      continue;
      
    applyCamera(&trapRect, camera);
    
    /* Set color based on trap type */
    switch (trap->type) {
    case TRAP_TYPE_FUEL_BARREL:
      if (trap->state == TRAP_STATE_TRIGGERED) {
        /* Explosion effect - orange/red */
        SDL_SetRenderDrawColor(renderer, 255, 100, 0, 200);
        /* Draw explosion circle */
        int radius = (int)trap->effectRadius;
        SDL_Rect explosion = {trapRect.x - radius, trapRect.y - radius, 
                              radius * 2, radius * 2};
        SDL_RenderFillRect(renderer, &explosion);
      } else if (trap->state != TRAP_STATE_DESTROYED) {
        /* Barrel - dark red */
        SDL_SetRenderDrawColor(renderer, 180, 50, 50, 255);
        SDL_RenderFillRect(renderer, &trapRect);
        /* Hazard stripe */
        SDL_SetRenderDrawColor(renderer, 255, 200, 0, 255);
        SDL_Rect stripe = {trapRect.x, trapRect.y + trapRect.h / 3, 
                           trapRect.w, trapRect.h / 3};
        SDL_RenderFillRect(renderer, &stripe);
      }
      break;
      
    case TRAP_TYPE_SPIKE_PIT:
      /* Spikes - gray */
      SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
      SDL_RenderFillRect(renderer, &trapRect);
      /* Draw spike triangles as lines */
      SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
      for (int x = trapRect.x + 5; x < trapRect.x + trapRect.w - 5; x += 10) {
        SDL_RenderDrawLine(renderer, x, trapRect.y + trapRect.h, 
                           x + 5, trapRect.y);
        SDL_RenderDrawLine(renderer, x + 5, trapRect.y, 
                           x + 10, trapRect.y + trapRect.h);
      }
      break;
      
    case TRAP_TYPE_ELECTRIC_FENCE:
      /* Electric - blue */
      SDL_SetRenderDrawColor(renderer, 50, 150, 255, 255);
      SDL_RenderFillRect(renderer, &trapRect);
      if (trap->state == TRAP_STATE_ACTIVE) {
        /* Sparking effect */
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 200);
        SDL_RenderFillRect(renderer, &trapRect);
      }
      break;
      
    case TRAP_TYPE_FIRE_GRATE:
      /* Fire - orange */
      SDL_SetRenderDrawColor(renderer, 255, 100, 0, 255);
      SDL_RenderFillRect(renderer, &trapRect);
      break;
    }
  }
}

// ---------------------------------------------------------------------------
// Flag Rendering
// ---------------------------------------------------------------------------

static void renderFlags(SDL_Renderer *renderer, TTF_Font *font,
                         const FlagArray *flags, const SDL_Rect *camera) {
  if (!flags)
    return;
    
  for (size_t i = 0; i < flags->count; i++) {
    const Flag *flag = &flags->flags[i];
    SDL_Rect flagRect = flag->rect;
    
    if (!isRectVisible(&flagRect, camera))
      continue;
      
    applyCamera(&flagRect, camera);
    
    /* Draw capture zone */
    if (flag->state == FLAG_STATE_CAPTURED) {
      SDL_SetRenderDrawColor(renderer, 0, 255, 0, 100);
    } else if (flag->state == FLAG_STATE_CAPTURING) {
      SDL_SetRenderDrawColor(renderer, 255, 255, 0, 100);
    } else {
      SDL_SetRenderDrawColor(renderer, 255, 255, 255, 50);
    }
    SDL_RenderFillRect(renderer, &flagRect);
    
    /* Draw flag pole */
    SDL_SetRenderDrawColor(renderer, 139, 90, 43, 255);
    int poleX = flagRect.x + flagRect.w / 2;
    int poleY = flagRect.y;
    SDL_Rect pole = {poleX - 2, poleY - 60, 4, 60};
    SDL_RenderFillRect(renderer, &pole);
    
    /* Draw flag */
    if (flag->state == FLAG_STATE_CAPTURED) {
      SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    } else {
      SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    }
    SDL_Rect flagSymbol = {poleX + 2, poleY - 60, 30, 20};
    SDL_RenderFillRect(renderer, &flagSymbol);
    
    /* Draw capture progress bar if capturing */
    if (flag->state == FLAG_STATE_CAPTURING) {
      SDL_Rect progressBg = {flagRect.x, flagRect.y - 15, flagRect.w, 10};
      SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
      SDL_RenderFillRect(renderer, &progressBg);
      
      int progressWidth = (int)(flagRect.w * flag->captureProgress);
      SDL_Rect progressFill = {flagRect.x, flagRect.y - 15, progressWidth, 10};
      SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
      SDL_RenderFillRect(renderer, &progressFill);
    }
  }
  
  (void)font; /* Unused for now, could be used for labels */
}

// ---------------------------------------------------------------------------
// Grenade Rendering
// ---------------------------------------------------------------------------

static void renderGrenades(SDL_Renderer *renderer, const GrenadePool *pool,
                            const SDL_Rect *camera) {
  if (!pool)
    return;
    
  for (int i = 0; i < MAX_GRENADES; i++) {
    const Grenade *grenade = pool->grenades[i];
    if (!grenade || !isGrenadeActive(grenade))
      continue;
      
    SDL_Rect grenadeRect = {(int)grenade->base.pos.x - 8, 
                             (int)grenade->base.pos.y - 8, 16, 16};
    
    if (!isRectVisible(&grenadeRect, camera))
      continue;
      
    applyCamera(&grenadeRect, camera);
    
    if (isGrenadeExploding(grenade)) {
      /* Draw explosion */
      int radius = (int)grenade->explosionRadius;
      SDL_Rect explosion = {grenadeRect.x - radius + 8, 
                            grenadeRect.y - radius + 8,
                            radius * 2, radius * 2};
      
      /* Multiple circles for explosion effect */
      SDL_SetRenderDrawColor(renderer, 255, 200, 0, 200);
      SDL_RenderFillRect(renderer, &explosion);
      
      SDL_Rect innerExplosion = {explosion.x + radius / 3, 
                                  explosion.y + radius / 3,
                                  radius * 4 / 3, radius * 4 / 3};
      SDL_SetRenderDrawColor(renderer, 255, 100, 0, 255);
      SDL_RenderFillRect(renderer, &innerExplosion);
      
      SDL_Rect coreExplosion = {explosion.x + radius / 2, 
                                 explosion.y + radius / 2,
                                 radius, radius};
      SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
      SDL_RenderFillRect(renderer, &coreExplosion);
    } else {
      /* Draw grenade */
      SDL_SetRenderDrawColor(renderer, 50, 100, 50, 255);
      SDL_RenderFillRect(renderer, &grenadeRect);
      
      /* Draw pin/fuse indicator based on remaining time */
      if (grenade->fuseTimer < 0.5f) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
      } else {
        SDL_SetRenderDrawColor(renderer, 200, 150, 50, 255);
      }
      SDL_Rect fuse = {grenadeRect.x + 6, grenadeRect.y - 4, 4, 6};
      SDL_RenderFillRect(renderer, &fuse);
    }
  }
}

// ---------------------------------------------------------------------------
// Grenade Count UI
// ---------------------------------------------------------------------------

static void renderGrenadeCount(SDL_Renderer *renderer, TTF_Font *font,
                                const Player *player) {
  if (!player)
    return;
    
  char grenadeText[32];
  snprintf(grenadeText, sizeof(grenadeText), "Grenades: %d", player->grenadeCount);
  
  SDL_Color textColor = {255, 255, 255, 255};
  renderText(renderer, font, grenadeText, HEALTH_BAR_OFFSET_X, 
             HEALTH_BAR_OFFSET_Y + 60, textColor);
}

// ---------------------------------------------------------------------------
// Score UI
// ---------------------------------------------------------------------------

static void renderScore(SDL_Renderer *renderer, TTF_Font *font,
                         const ScoreTracker *tracker) {
  if (!tracker)
    return;
    
  char scoreText[32];
  snprintf(scoreText, sizeof(scoreText), "Score: %d", tracker->currentScore);
  
  SDL_Color textColor = {255, 255, 0, 255};
  renderText(renderer, font, scoreText, TIMER_DISPLAY_X - 150, 
             TIMER_DISPLAY_Y, textColor);
}

// ---------------------------------------------------------------------------
// Combo Display
// ---------------------------------------------------------------------------

static void renderCombo(SDL_Renderer *renderer, TTF_Font *font,
                         const ScoreTracker *tracker) {
  if (!tracker || tracker->currentCombo <= 1)
    return;
    
  char comboText[32];
  snprintf(comboText, sizeof(comboText), "Combo x%d!", tracker->currentCombo);
  
  SDL_Color textColor = {255, 100, 0, 255};
  renderText(renderer, font, comboText, GAME_WIDTH / 2 - 50, 100, textColor);
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

  // Render the map platforms.
  renderMapPlatforms(ctx->renderer, ctx->map, &camera);

  // Render ladders (behind entities).
  if (ctx->map->ladders) {
    renderLadders(ctx->renderer, (LadderArray *)ctx->map->ladders, &camera);
  }

  // Render traps.
  if (ctx->map->traps) {
    renderTraps(ctx->renderer, (TrapArray *)ctx->map->traps, &camera);
  }

  // Render flags.
  if (ctx->map->flags) {
    renderFlags(ctx->renderer, ctx->texManager->font, 
                (FlagArray *)ctx->map->flags, &camera);
  }

  // Render game entities relative to the camera.
  playerDraw(ctx->player, ctx->renderer, ctx->texManager, &camera);
  enemyArrayDraw(ctx->enemies, ctx->renderer, ctx->texManager, &camera);
  bulletPoolDraw(ctx->bulletPool, ctx->renderer, ctx->texManager, &camera);

  // Render grenades.
  if (ctx->grenadePool) {
    renderGrenades(ctx->renderer, ctx->grenadePool, &camera);
  }

  // Render the UI elements.
  renderHealthBar(ctx->renderer, ctx->player, ctx->texManager->font);
  renderGrenadeCount(ctx->renderer, ctx->texManager->font, ctx->player);
  
  // Render score and timer.
  if (ctx->scoreTracker) {
    renderTimer(ctx->renderer, ctx->texManager->font, ctx->scoreTracker);
    renderScore(ctx->renderer, ctx->texManager->font, ctx->scoreTracker);
    renderCombo(ctx->renderer, ctx->texManager->font, ctx->scoreTracker);
  }

  // Present the final rendered frame.
  SDL_RenderPresent(ctx->renderer);
}

