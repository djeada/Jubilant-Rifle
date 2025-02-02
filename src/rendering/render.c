#include "rendering/render.h"
#include "utils/consts.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h> // Needed if you are rendering text

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
  // Calculate scaled dimensions
  const int bgWidth = HEALTH_BAR_BG_WIDTH * HEALTH_BAR_SCALE;
  const int bgHeight = HEALTH_BAR_BG_HEIGHT * HEALTH_BAR_SCALE;
  const int barWidth = HEALTH_BAR_WIDTH * HEALTH_BAR_SCALE;
  const int barHeight = HEALTH_BAR_HEIGHT * HEALTH_BAR_SCALE;

  // Position the health bar at a fixed offset from the top-left corner.
  SDL_Rect bgRect = {.x = HEALTH_BAR_OFFSET_X,
                     .y = HEALTH_BAR_OFFSET_Y,
                     .w = bgWidth,
                     .h = bgHeight};

  // Render the background rectangle (e.g. a dark grey)
  SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
  SDL_RenderFillRect(renderer, &bgRect);

  // Calculate how much of the bar should be filled.
  // (Ensure maxHealth is not zero to avoid division by zero.)
  float healthRatio = 100 > 0 ? (float)player->base.health / 100 : 0;
  int filledBarWidth = (int)(barWidth * healthRatio);

  // Center the bar inside the background (if needed)
  SDL_Rect healthRect = {.x = bgRect.x + (bgWidth - barWidth) / 2,
                         .y = bgRect.y + (bgHeight - barHeight) / 2,
                         .w = filledBarWidth,
                         .h = barHeight};

  // Render the filled portion of the bar (e.g. red)
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  SDL_RenderFillRect(renderer, &healthRect);

  // Optionally draw a border around the background (white)
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderDrawRect(renderer, &bgRect);

  // Render the level text. For example: "Level: 3"
  char levelText[32];
  snprintf(levelText, sizeof(levelText), "%s%d", HEALTH_BAR_TEXT,
           player->level);

  // Choose a text color (white)
  SDL_Color textColor = {255, 255, 255, 255};

  // Render the text below the health bar (you can adjust the position as
  // needed)
  int textX = HEALTH_BAR_OFFSET_X;
  int textY = HEALTH_BAR_OFFSET_Y + bgHeight + 5; // 5 pixels gap
  renderText(renderer, font, levelText, textX, textY, textColor);
}

void render(SDL_Renderer *renderer, TextureManager *tm, Player *player,
            BulletPool *pool, EnemyArray *arr) {
  // Clear the renderer with a default color
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  // --- Render the background texture ---
  SDL_Rect bgRect = {0, 0, GAME_WIDTH, GAME_HEIGHT};
  SDL_RenderCopy(renderer, tm->backgroundTex, NULL, &bgRect);

  // Render game elements on top of the background
  playerDraw(player, renderer, tm);
  enemyArrayDraw(arr, renderer, tm);
  bulletPoolDraw(pool, renderer, tm);

  // Render the player's health bar and level text on top of everything else.
  renderHealthBar(renderer, player, tm->font);

  // Present the final rendered frame
  SDL_RenderPresent(renderer);
}
