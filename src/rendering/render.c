#include "rendering/render.h"
#include "entities/entity.h"
#include "entities/grenade.h"
#include "entities/grenade_pool.h"
#include "game/score.h"
#include "map/flag.h"
#include "map/ladder.h"
#include "map/map.h"
#include "map/trap.h"
#include "utils/consts.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include <stddef.h>

/* ---------------------------------------------------------------------------
 * Rendering Constants
 * --------------------------------------------------------------------------- */

/** Ladder visual properties */
#define LADDER_SIDE_WIDTH 5
#define LADDER_RUNG_SPACING 20
#define LADDER_RUNG_MARGIN 10
#define LADDER_RUNG_HEIGHT 4

/** Color values for entities */
#define PLATFORM_COLOR_R 100
#define PLATFORM_COLOR_G 100
#define PLATFORM_COLOR_B 100

#define LADDER_COLOR_R 139
#define LADDER_COLOR_G 90
#define LADDER_COLOR_B 43

/* ---------------------------------------------------------------------------
 * Utility Inline Functions
 * --------------------------------------------------------------------------- */

/**
 * @brief Check if an entity rectangle is visible within the camera view.
 * @param entityRect The entity's bounding rectangle.
 * @param camera The camera's viewport rectangle.
 * @return Non-zero if visible, zero otherwise.
 */
static inline int isRectVisible(const SDL_Rect *entityRect,
                                const SDL_Rect *camera) {
  return SDL_HasIntersection(entityRect, camera);
}

/**
 * @brief Transform world coordinates to screen coordinates.
 * @param dest Rectangle to transform (modified in place).
 * @param camera The camera's viewport rectangle.
 */
static inline void applyCamera(SDL_Rect *dest, const SDL_Rect *camera) {
  dest->x -= camera->x;
  dest->y -= camera->y;
}

/**
 * @brief Draw a filled circle using scanline rendering.
 * @param renderer SDL renderer to draw with.
 * @param cx Center X coordinate.
 * @param cy Center Y coordinate.
 * @param radius Circle radius.
 */
static void drawFilledCircle(SDL_Renderer *renderer, int cx, int cy,
                             int radius) {
  if (radius <= 0)
    return;

  const int radiusSq = radius * radius;
  for (int y = -radius; y <= radius; y++) {
    const int ySq = y * y;
    const int width = (int)(sqrt((double)(radiusSq - ySq)) * 2.0);
    if (width > 0) {
      const SDL_Rect line = {cx - width / 2, cy + y, width, 1};
      SDL_RenderFillRect(renderer, &line);
    }
  }
}

/* ---------------------------------------------------------------------------
 * Entity Rendering Helpers
 * --------------------------------------------------------------------------- */

/**
 * @brief Render an animated entity with sprite sheet support.
 */
static void renderAnimatedEntity(SDL_Renderer *renderer, SDL_Texture *tex,
                                 const SDL_Rect *dest, const Animation *anim,
                                 int frameW, int frameH, Direction direction) {
  if (!renderer || !tex || !dest)
    return;

  SDL_Rect src = {0, 0, frameW, frameH};
  if (anim && anim->frameCount > 0) {
    src.x = anim->currentFrame * frameW;
  }

  const SDL_RendererFlip flip =
      (direction == DIRECTION_LEFT) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
  SDL_RenderCopyEx(renderer, tex, &src, dest, 0.0, NULL, flip);
}

/* ---------------------------------------------------------------------------
 * Entity Draw Functions
 * --------------------------------------------------------------------------- */

/**
 * @brief Draw the player entity.
 */
static void playerDraw(const Player *restrict player, SDL_Renderer *renderer,
                       const TextureManager *restrict tm,
                       const SDL_Rect *camera) {
  if (!player || !renderer || !tm)
    return;

  const SDL_Rect playerRect = {(int)player->base.pos.x,
                               (int)player->base.pos.y, SPRITE_WIDTH,
                               HUMANOID_FRAME_HEIGHT};

  if (!isRectVisible(&playerRect, camera))
    return;

  SDL_Rect playerDest = playerRect;
  applyCamera(&playerDest, camera);

  const SDL_RendererFlip flip = (player->base.direction == DIRECTION_LEFT)
                                    ? SDL_FLIP_HORIZONTAL
                                    : SDL_FLIP_NONE;

  if (player->base.anim && player->base.anim->frameCount > 0) {
    const SDL_Rect src = {player->base.anim->currentFrame * SPRITE_WIDTH, 0,
                          SPRITE_WIDTH, HUMANOID_FRAME_HEIGHT};
    SDL_RenderCopyEx(renderer, tm->playerTex, &src, &playerDest, 0.0, NULL,
                     flip);
  } else {
    SDL_RenderCopyEx(renderer, tm->playerTex, NULL, &playerDest, 0.0, NULL,
                     flip);
  }
}

/**
 * @brief Draw all active bullets from the bullet pool.
 */
static void bulletPoolDraw(const BulletPool *restrict pool,
                           SDL_Renderer *renderer,
                           const TextureManager *restrict tm,
                           const SDL_Rect *camera) {
  if (!pool || !renderer || !tm)
    return;

  /* Draw player bullets */
  for (int i = 0; i < pool->playerActiveCount; i++) {
    const int index = pool->playerActiveIndices[i];
    const Bullet *restrict bullet = pool->playerBullets[index];

    if (!bullet || !isBulletAlive(bullet))
      continue;

    const SDL_Rect bulletRect = {(int)bullet->base.pos.x,
                                 (int)bullet->base.pos.y, BULLET_SPRITE_WIDTH,
                                 BULLET_SPRITE_HEIGHT};

    if (!isRectVisible(&bulletRect, camera))
      continue;

    SDL_Rect dest = bulletRect;
    applyCamera(&dest, camera);

    const double angle = bulletGetRotation(bullet);
    const SDL_Point center = {BULLET_SPRITE_WIDTH / 2, BULLET_SPRITE_HEIGHT / 2};
    SDL_RenderCopyEx(renderer, tm->bulletTex, NULL, &dest, angle, &center,
                     SDL_FLIP_NONE);
  }

  /* Draw enemy bullets */
  for (int i = 0; i < pool->enemyActiveCount; i++) {
    const int index = pool->enemyActiveIndices[i];
    const Bullet *restrict bullet = pool->enemyBullets[index];

    if (!bullet || !isBulletAlive(bullet))
      continue;

    const SDL_Rect bulletRect = {(int)bullet->base.pos.x,
                                 (int)bullet->base.pos.y, BULLET_SPRITE_WIDTH,
                                 BULLET_SPRITE_HEIGHT};

    if (!isRectVisible(&bulletRect, camera))
      continue;

    SDL_Rect dest = bulletRect;
    applyCamera(&dest, camera);

    const double angle = bulletGetRotation(bullet);
    const SDL_Point center = {BULLET_SPRITE_WIDTH / 2, BULLET_SPRITE_HEIGHT / 2};
    SDL_RenderCopyEx(renderer, tm->bulletTex, NULL, &dest, angle, &center,
                     SDL_FLIP_NONE);
  }
}

/**
 * @brief Draw all enemies in the enemy array.
 */
static void enemyArrayDraw(const EnemyArray *restrict arr,
                           SDL_Renderer *renderer,
                           const TextureManager *restrict tm,
                           const SDL_Rect *camera) {
  if (!arr || !renderer || !tm)
    return;

  for (int i = 0; i < arr->count; i++) {
    const Entity *restrict enemy = &arr->data[i]->base;

    if (!isEntityAlive(enemy))
      continue;

    const SDL_Rect enemyRect = {(int)enemy->pos.x, (int)enemy->pos.y,
                                SPRITE_WIDTH, HUMANOID_FRAME_HEIGHT};

    if (!isRectVisible(&enemyRect, camera))
      continue;

    SDL_Rect dest = enemyRect;
    applyCamera(&dest, camera);
    renderAnimatedEntity(renderer, tm->enemyTex, &dest, enemy->anim,
                         SPRITE_WIDTH, HUMANOID_FRAME_HEIGHT, enemy->direction);
  }
}

/* ---------------------------------------------------------------------------
 * Platform Rendering
 * --------------------------------------------------------------------------- */

/**
 * @brief Render all map platforms.
 */
static void renderMapPlatforms(SDL_Renderer *renderer, const Map *restrict map,
                               const SDL_Rect *camera) {
  if (!renderer || !map || !map->platforms)
    return;

  SDL_SetRenderDrawColor(renderer, PLATFORM_COLOR_R, PLATFORM_COLOR_G,
                         PLATFORM_COLOR_B, 255);

  for (size_t i = 0; i < map->platformCount; i++) {
    SDL_Rect platformRect = map->platforms[i];

    if (!isRectVisible(&platformRect, camera))
      continue;

    applyCamera(&platformRect, camera);
    SDL_RenderFillRect(renderer, &platformRect);
  }
}

/* ---------------------------------------------------------------------------
 * UI Rendering Functions (screen-space, not affected by camera)
 * --------------------------------------------------------------------------- */

/**
 * @brief Render text at a specified position.
 */
static void renderText(SDL_Renderer *renderer, TTF_Font *font, const char *text,
                       int x, int y, SDL_Color color) {
  if (!renderer || !font || !text)
    return;

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

  const SDL_Rect dstRect = {x, y, surface->w, surface->h};
  SDL_RenderCopy(renderer, texture, NULL, &dstRect);
  SDL_DestroyTexture(texture);
  SDL_FreeSurface(surface);
}

/**
 * @brief Render the player health bar.
 */
static void renderHealthBar(SDL_Renderer *renderer,
                            const Player *restrict player, TTF_Font *font) {
  if (!renderer || !player)
    return;

  const int bgWidth = HEALTH_BAR_BG_WIDTH * HEALTH_BAR_SCALE;
  const int bgHeight = HEALTH_BAR_BG_HEIGHT * HEALTH_BAR_SCALE;
  const int barWidth = HEALTH_BAR_WIDTH * HEALTH_BAR_SCALE;
  const int barHeight = HEALTH_BAR_HEIGHT * HEALTH_BAR_SCALE;

  const SDL_Rect bgRect = {HEALTH_BAR_OFFSET_X, HEALTH_BAR_OFFSET_Y, bgWidth,
                           bgHeight};

  /* Draw background */
  SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
  SDL_RenderFillRect(renderer, &bgRect);

  /* Calculate health fill */
  const float healthRatio =
      (player->base.health > 0) ? (float)player->base.health / 100.0f : 0.0f;
  const int filledBarWidth = (int)(barWidth * healthRatio);

  const SDL_Rect healthRect = {bgRect.x + (bgWidth - barWidth) / 2,
                               bgRect.y + (bgHeight - barHeight) / 2,
                               filledBarWidth, barHeight};

  /* Draw health fill */
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  SDL_RenderFillRect(renderer, &healthRect);

  /* Draw border */
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderDrawRect(renderer, &bgRect);

  /* Draw level text */
  if (font) {
    char levelText[32];
    snprintf(levelText, sizeof(levelText), "%s%d", HEALTH_BAR_TEXT,
             player->level);
    const SDL_Color textColor = {255, 255, 255, 255};
    const int textX = HEALTH_BAR_OFFSET_X;
    const int textY = HEALTH_BAR_OFFSET_Y + bgHeight + 5;
    renderText(renderer, font, levelText, textX, textY, textColor);
  }
}

/* ---------------------------------------------------------------------------
 * Level Timer Rendering
 * --------------------------------------------------------------------------- */

/**
 * @brief Render the level timer.
 */
static void renderTimer(SDL_Renderer *renderer, TTF_Font *font,
                        const ScoreTracker *tracker) {
  if (!renderer || !font || !tracker)
    return;

  char timeStr[16];
  scoreGetElapsedTimeString(tracker, timeStr, sizeof(timeStr));

  const SDL_Color textColor = {255, 255, 255, 255};
  renderText(renderer, font, timeStr, TIMER_DISPLAY_X, TIMER_DISPLAY_Y,
             textColor);
}

/* ---------------------------------------------------------------------------
 * Ladder Rendering
 * --------------------------------------------------------------------------- */

/**
 * @brief Render all ladders.
 */
static void renderLadders(SDL_Renderer *renderer,
                          const LadderArray *restrict ladders,
                          const SDL_Rect *camera) {
  if (!renderer || !ladders)
    return;

  SDL_SetRenderDrawColor(renderer, LADDER_COLOR_R, LADDER_COLOR_G,
                         LADDER_COLOR_B, 255);

  for (size_t i = 0; i < ladders->count; i++) {
    SDL_Rect ladderRect = ladders->ladders[i].rect;

    if (!isRectVisible(&ladderRect, camera))
      continue;

    applyCamera(&ladderRect, camera);

    /* Draw ladder sides */
    const SDL_Rect leftSide = {ladderRect.x, ladderRect.y, LADDER_SIDE_WIDTH,
                               ladderRect.h};
    const SDL_Rect rightSide = {ladderRect.x + ladderRect.w - LADDER_SIDE_WIDTH,
                                ladderRect.y, LADDER_SIDE_WIDTH, ladderRect.h};
    SDL_RenderFillRect(renderer, &leftSide);
    SDL_RenderFillRect(renderer, &rightSide);

    /* Draw rungs */
    const int rungEndY = ladderRect.y + ladderRect.h - LADDER_RUNG_MARGIN;
    for (int y = ladderRect.y + LADDER_RUNG_MARGIN; y < rungEndY;
         y += LADDER_RUNG_SPACING) {
      const SDL_Rect rung = {ladderRect.x + LADDER_SIDE_WIDTH, y,
                             ladderRect.w - 2 * LADDER_SIDE_WIDTH,
                             LADDER_RUNG_HEIGHT};
      SDL_RenderFillRect(renderer, &rung);
    }
  }
}

/* ---------------------------------------------------------------------------
 * Trap Rendering
 * --------------------------------------------------------------------------- */

/**
 * @brief Render all traps.
 */
static void renderTraps(SDL_Renderer *renderer,
                        const TrapArray *restrict traps,
                        const SDL_Rect *camera) {
  if (!renderer || !traps)
    return;

  for (size_t i = 0; i < traps->count; i++) {
    const Trap *restrict trap = &traps->traps[i];
    SDL_Rect trapRect = trap->rect;

    if (!isRectVisible(&trapRect, camera))
      continue;

    applyCamera(&trapRect, camera);

    switch (trap->type) {
    case TRAP_TYPE_FUEL_BARREL:
      if (trap->state == TRAP_STATE_TRIGGERED) {
        /* Enhanced explosion effect */
        const int radius = (int)trap->effectRadius;
        const int cx = trapRect.x + trapRect.w / 2;
        const int cy = trapRect.y + trapRect.h / 2;

        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

        /* Outer blast */
        SDL_SetRenderDrawColor(renderer, 255, 150, 0, 180);
        drawFilledCircle(renderer, cx, cy, radius);

        /* Middle flame ring */
        SDL_SetRenderDrawColor(renderer, 255, 200, 50, 220);
        drawFilledCircle(renderer, cx, cy, radius * 2 / 3);

        /* Hot core */
        SDL_SetRenderDrawColor(renderer, 255, 255, 100, 255);
        drawFilledCircle(renderer, cx, cy, radius / 3);

        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
      } else if (trap->state != TRAP_STATE_DESTROYED) {
        /* Barrel body - dark red */
        SDL_SetRenderDrawColor(renderer, 180, 50, 50, 255);
        SDL_RenderFillRect(renderer, &trapRect);

        /* Hazard stripe */
        SDL_SetRenderDrawColor(renderer, 255, 200, 0, 255);
        const SDL_Rect stripe = {trapRect.x, trapRect.y + trapRect.h / 3,
                                 trapRect.w, trapRect.h / 3};
        SDL_RenderFillRect(renderer, &stripe);
      }
      break;

    case TRAP_TYPE_SPIKE_PIT:
      /* Base - gray */
      SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
      SDL_RenderFillRect(renderer, &trapRect);

      /* Draw spike triangles */
      SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
      for (int x = trapRect.x + 5; x < trapRect.x + trapRect.w - 5; x += 10) {
        SDL_RenderDrawLine(renderer, x, trapRect.y + trapRect.h, x + 5,
                           trapRect.y);
        SDL_RenderDrawLine(renderer, x + 5, trapRect.y, x + 10,
                           trapRect.y + trapRect.h);
      }
      break;

    case TRAP_TYPE_ELECTRIC_FENCE:
      /* Electric - blue */
      SDL_SetRenderDrawColor(renderer, 50, 150, 255, 255);
      SDL_RenderFillRect(renderer, &trapRect);

      if (trap->state == TRAP_STATE_ACTIVE) {
        /* Sparking effect - white flash */
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

/* ---------------------------------------------------------------------------
 * Flag Rendering
 * --------------------------------------------------------------------------- */

/**
 * @brief Render all flags and capture objectives.
 */
static void renderFlags(SDL_Renderer *renderer, TTF_Font *font,
                        const FlagArray *restrict flags,
                        const SDL_Rect *camera) {
  if (!renderer || !flags)
    return;

  for (size_t i = 0; i < flags->count; i++) {
    const Flag *restrict flag = &flags->flags[i];
    SDL_Rect flagRect = flag->rect;

    if (!isRectVisible(&flagRect, camera))
      continue;

    applyCamera(&flagRect, camera);

    /* Draw capture zone with state-based color */
    switch (flag->state) {
    case FLAG_STATE_CAPTURED:
      SDL_SetRenderDrawColor(renderer, 0, 255, 0, 100);
      break;
    case FLAG_STATE_CAPTURING:
      SDL_SetRenderDrawColor(renderer, 255, 255, 0, 100);
      break;
    default:
      SDL_SetRenderDrawColor(renderer, 255, 255, 255, 50);
      break;
    }
    SDL_RenderFillRect(renderer, &flagRect);

    /* Draw flag pole */
    SDL_SetRenderDrawColor(renderer, LADDER_COLOR_R, LADDER_COLOR_G,
                           LADDER_COLOR_B, 255);
    const int poleX = flagRect.x + flagRect.w / 2;
    const int poleY = flagRect.y;
    const SDL_Rect pole = {poleX - 2, poleY - 60, 4, 60};
    SDL_RenderFillRect(renderer, &pole);

    /* Draw flag symbol */
    if (flag->state == FLAG_STATE_CAPTURED) {
      SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    } else {
      SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    }
    const SDL_Rect flagSymbol = {poleX + 2, poleY - 60, 30, 20};
    SDL_RenderFillRect(renderer, &flagSymbol);

    /* Draw capture progress bar if capturing */
    if (flag->state == FLAG_STATE_CAPTURING) {
      const SDL_Rect progressBg = {flagRect.x, flagRect.y - 15, flagRect.w, 10};
      SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
      SDL_RenderFillRect(renderer, &progressBg);

      const int progressWidth = (int)(flagRect.w * flag->captureProgress);
      const SDL_Rect progressFill = {flagRect.x, flagRect.y - 15, progressWidth,
                                     10};
      SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
      SDL_RenderFillRect(renderer, &progressFill);
    }
  }

  (void)font; /* Reserved for future label rendering */
}

/* ---------------------------------------------------------------------------
 * Grenade Rendering
 * --------------------------------------------------------------------------- */

/**
 * @brief Render all active grenades.
 */
static void renderGrenades(SDL_Renderer *renderer,
                           const GrenadePool *restrict pool,
                           const SDL_Rect *camera) {
  if (!renderer || !pool)
    return;

  for (int i = 0; i < MAX_GRENADES; i++) {
    const Grenade *restrict grenade = pool->grenades[i];

    if (!grenade || !isGrenadeActive(grenade))
      continue;

    SDL_Rect grenadeRect = {(int)grenade->base.pos.x - 8,
                            (int)grenade->base.pos.y - 8, 16, 16};

    if (!isRectVisible(&grenadeRect, camera))
      continue;

    applyCamera(&grenadeRect, camera);

    if (isGrenadeExploding(grenade)) {
      /* Explosion animation with dynamic sizing and color */
      const float progress =
          1.0f - (grenade->explosionTimer / GRENADE_EXPLOSION_DURATION);
      const int baseRadius = (int)grenade->explosionRadius;

      /* Calculate layer radii */
      const int outerRadius = (int)(baseRadius * (0.5f + progress * 1.5f));
      const int midRadius = (int)(baseRadius * (0.7f + progress * 0.8f));
      const int coreRadius = (int)(baseRadius * (0.5f + progress * 0.3f));
      const int alpha = (int)(255.0f * (1.0f - progress));

      const int centerX = grenadeRect.x + 8;
      const int centerY = grenadeRect.y + 8;

      SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

      /* Outer expanding ring */
      SDL_SetRenderDrawColor(renderer, 255, 150, 0, (Uint8)(alpha / 2));
      drawFilledCircle(renderer, centerX, centerY, outerRadius);

      /* Middle blast wave */
      SDL_SetRenderDrawColor(renderer, 255, 200, 50, (Uint8)alpha);
      drawFilledCircle(renderer, centerX, centerY, midRadius);

      /* Inner hot core with white flash at start */
      if (progress < 0.3f) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
      } else {
        SDL_SetRenderDrawColor(renderer, 255, 100, 0, (Uint8)alpha);
      }
      drawFilledCircle(renderer, centerX, centerY, coreRadius);

      SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    } else {
      /* Grenade body */
      SDL_SetRenderDrawColor(renderer, 40, 80, 40, 255);
      SDL_RenderFillRect(renderer, &grenadeRect);

      /* Highlight for depth */
      const SDL_Rect highlight = {grenadeRect.x + 2, grenadeRect.y + 2, 6, 6};
      SDL_SetRenderDrawColor(renderer, 70, 120, 70, 255);
      SDL_RenderFillRect(renderer, &highlight);

      /* Pulsing warning when near detonation */
      const float fuseProgress = grenade->fuseTimer / GRENADE_FUSE_TIME;
      if (fuseProgress < 0.3f) {
        const int pulseAlpha =
            (int)(200.0f + 55.0f * sinf(grenade->fuseTimer * 20.0f));
        const int centerX = grenadeRect.x + 8;
        const int centerY = grenadeRect.y + 8;

        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, (Uint8)pulseAlpha);
        drawFilledCircle(renderer, centerX, centerY, 10);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
      }

      /* Draw fuse pin */
      SDL_SetRenderDrawColor(renderer, 200, 150, 50, 255);
      const SDL_Rect fuse = {grenadeRect.x + 6, grenadeRect.y - 4, 4, 6};
      SDL_RenderFillRect(renderer, &fuse);
    }
  }
}

/* ---------------------------------------------------------------------------
 * Grenade Count UI
 * --------------------------------------------------------------------------- */

/**
 * @brief Render the grenade count display.
 */
static void renderGrenadeCount(SDL_Renderer *renderer, TTF_Font *font,
                               const Player *restrict player) {
  if (!renderer || !font || !player)
    return;

  char grenadeText[32];
  snprintf(grenadeText, sizeof(grenadeText), "Grenades: %d",
           player->grenadeCount);

  const SDL_Color textColor = {255, 255, 255, 255};
  renderText(renderer, font, grenadeText, HEALTH_BAR_OFFSET_X,
             HEALTH_BAR_OFFSET_Y + 60, textColor);
}

/* ---------------------------------------------------------------------------
 * Score UI
 * --------------------------------------------------------------------------- */

/**
 * @brief Render the score display.
 */
static void renderScore(SDL_Renderer *renderer, TTF_Font *font,
                        const ScoreTracker *tracker) {
  if (!renderer || !font || !tracker)
    return;

  char scoreText[32];
  snprintf(scoreText, sizeof(scoreText), "Score: %d", tracker->currentScore);

  const SDL_Color textColor = {255, 255, 0, 255};
  renderText(renderer, font, scoreText, TIMER_DISPLAY_X - 150, TIMER_DISPLAY_Y,
             textColor);
}

/* ---------------------------------------------------------------------------
 * Combo Display
 * --------------------------------------------------------------------------- */

/**
 * @brief Render the combo multiplier display.
 */
static void renderCombo(SDL_Renderer *renderer, TTF_Font *font,
                        const ScoreTracker *tracker) {
  if (!renderer || !font || !tracker || tracker->currentCombo <= 1)
    return;

  char comboText[32];
  snprintf(comboText, sizeof(comboText), "Combo x%d!", tracker->currentCombo);

  const SDL_Color textColor = {255, 100, 0, 255};
  renderText(renderer, font, comboText, GAME_WIDTH / 2 - 50, 100, textColor);
}

/* ---------------------------------------------------------------------------
 * Main Render Function
 * --------------------------------------------------------------------------- */

/**
 * @brief Compute camera position clamped to map bounds.
 */
static SDL_Rect computeCamera(const GameContext *restrict ctx) {
  SDL_Rect camera;
  camera.w = GAME_WIDTH;
  camera.h = GAME_HEIGHT;

  /* Center camera on player */
  camera.x =
      (int)(ctx->player->base.pos.x + SPRITE_WIDTH / 2 - GAME_WIDTH / 2);
  camera.y = (int)(ctx->player->base.pos.y + HUMANOID_FRAME_HEIGHT / 2 -
                   GAME_HEIGHT / 2);

  /* Clamp to map bounds */
  const int mapRight = ctx->map->rect.x + ctx->map->rect.w - camera.w;
  const int mapBottom = ctx->map->rect.y + ctx->map->rect.h - camera.h;

  if (camera.x < ctx->map->rect.x)
    camera.x = ctx->map->rect.x;
  if (camera.y < ctx->map->rect.y)
    camera.y = ctx->map->rect.y;
  if (camera.x > mapRight)
    camera.x = mapRight;
  if (camera.y > mapBottom)
    camera.y = mapBottom;

  return camera;
}

void renderGame(GameContext *ctx) {
  if (!ctx || !ctx->renderer || !ctx->player || !ctx->map || !ctx->texManager)
    return;

  /* Compute camera view */
  const SDL_Rect camera = computeCamera(ctx);

  /* Clear the renderer */
  SDL_SetRenderDrawColor(ctx->renderer, 0, 0, 0, 255);
  SDL_RenderClear(ctx->renderer);

  /* Render background */
  if (ctx->texManager->backgroundTex) {
    const SDL_Rect bgSrc = camera;
    const SDL_Rect bgDest = {0, 0, GAME_WIDTH, GAME_HEIGHT};
    SDL_RenderCopy(ctx->renderer, ctx->texManager->backgroundTex, &bgSrc,
                   &bgDest);
  }

  /* Render map elements */
  renderMapPlatforms(ctx->renderer, ctx->map, &camera);

  /* Render ladders (behind entities) */
  if (ctx->map->ladders) {
    renderLadders(ctx->renderer, (const LadderArray *)ctx->map->ladders,
                  &camera);
  }

  /* Render traps */
  if (ctx->map->traps) {
    renderTraps(ctx->renderer, (const TrapArray *)ctx->map->traps, &camera);
  }

  /* Render flags */
  if (ctx->map->flags) {
    renderFlags(ctx->renderer, ctx->texManager->font,
                (const FlagArray *)ctx->map->flags, &camera);
  }

  /* Render game entities */
  playerDraw(ctx->player, ctx->renderer, ctx->texManager, &camera);

  if (ctx->enemies) {
    enemyArrayDraw(ctx->enemies, ctx->renderer, ctx->texManager, &camera);
  }

  if (ctx->bulletPool) {
    bulletPoolDraw(ctx->bulletPool, ctx->renderer, ctx->texManager, &camera);
  }

  /* Render grenades */
  if (ctx->grenadePool) {
    renderGrenades(ctx->renderer, ctx->grenadePool, &camera);
  }

  /* Render UI elements (screen-space) */
  renderHealthBar(ctx->renderer, ctx->player, ctx->texManager->font);
  renderGrenadeCount(ctx->renderer, ctx->texManager->font, ctx->player);

  /* Render score and timer */
  if (ctx->scoreTracker) {
    renderTimer(ctx->renderer, ctx->texManager->font, ctx->scoreTracker);
    renderScore(ctx->renderer, ctx->texManager->font, ctx->scoreTracker);
    renderCombo(ctx->renderer, ctx->texManager->font, ctx->scoreTracker);
  }

  /* Present the final rendered frame */
  SDL_RenderPresent(ctx->renderer);
}
