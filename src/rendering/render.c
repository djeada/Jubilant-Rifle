#include "rendering/render.h"
#include "entities/bullet.h"
#include "entities/enemy.h"
#include "utils/consts.h"
#include "utils/resources.h"
#include "utils/utils.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include <stdbool.h>

void setRenderLogicalSize(SDL_Renderer *renderer, int windowWidth,
                          int windowHeight) {
  float gameAspectRatio = (float)GAME_WIDTH / (float)GAME_HEIGHT;
  float windowAspectRatio = (float)windowWidth / (float)windowHeight;
  int rendererWidth, rendererHeight;

  if (windowAspectRatio < gameAspectRatio) {
    rendererWidth = windowWidth;
    rendererHeight = (int)(windowWidth / gameAspectRatio);
  } else {
    rendererHeight = windowHeight;
    rendererWidth = (int)(windowHeight * gameAspectRatio);
  }

  SDL_RenderSetLogicalSize(renderer, rendererWidth, rendererHeight);
}

void renderMap(SDL_Renderer *renderer, Map *map) {
  SDL_Surface *bgSurface = IMG_Load(map->backgroundImage);
  if (!bgSurface) {
    logError("Unable to load image");
    return;
  }

  SDL_Texture *bgTexture = SDL_CreateTextureFromSurface(renderer, bgSurface);
  SDL_FreeSurface(bgSurface);

  if (!bgTexture) {
    logError("Unable to create texture from surface");
    return;
  }

  SDL_RenderCopy(renderer, bgTexture, NULL, NULL);
  SDL_DestroyTexture(bgTexture);
}

void renderBullets(SDL_Renderer *renderer, BulletManager *bulletManager,
                   Camera *camera) {
  for (size_t i = 0; i < bulletManager->bullets.size; i++) {
    Bullet *bullet = (Bullet *)bulletManager->bullets.items[i];
    if (bullet && bullet->animation.isVisible) {
      int sheetX = (bullet->animation.currentSpriteIndex % SPRITES_PER_ROW) *
                   BULLET_SPRITE_WIDTH;
      int sheetY = (bullet->animation.currentSpriteIndex / SPRITES_PER_ROW) *
                   BULLET_SPRITE_HEIGHT;

      SDL_Rect sourceRect = {sheetX, sheetY, BULLET_SPRITE_WIDTH,
                             BULLET_SPRITE_HEIGHT};
      SDL_Rect destinationRect = {bullet->movement.position.x - camera->x,
                                  bullet->movement.position.y - camera->y,
                                  BULLET_SPRITE_WIDTH, BULLET_SPRITE_HEIGHT};

      if (SDL_RenderCopyEx(renderer, bullet->texture, &sourceRect,
                           &destinationRect, 0.0, NULL, SDL_FLIP_NONE) != 0) {
        logError("Unable to render bullet");
      }
    }
  }
}

void renderHumanoid(SDL_Renderer *renderer, Humanoid *humanoid,
                    Camera *camera) {
  if (!humanoidIsAlive(humanoid) || !humanoid->animation.isVisible)
    return;

  int sheetX =
      (humanoid->animation.currentSpriteIndex % SPRITES_PER_ROW) * SPRITE_WIDTH;
  int sheetY = (humanoid->animation.currentSpriteIndex / SPRITES_PER_ROW) *
               SPRITE_HEIGHT;

  SDL_Rect sourceRect = {sheetX, sheetY, SPRITE_WIDTH, SPRITE_HEIGHT};
  SDL_Rect destinationRect = {humanoid->movement.position.x - camera->x,
                              humanoid->movement.position.y - camera->y,
                              SPRITE_WIDTH, SPRITE_HEIGHT};

  SDL_RendererFlip flip =
      humanoid->animation.isFacingLeft ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

  if (SDL_RenderCopyEx(renderer, humanoid->texture, &sourceRect,
                       &destinationRect, 0.0, NULL, flip) != 0) {
    logError("Unable to render player");
  }

  renderBullets(renderer, &humanoid->bulletManager, camera);
}

void renderHealthBar(SDL_Renderer *renderer, Player *player) {
  unsigned int length = 100;
  SDL_Rect healthBarBackground = {10 + HEALTH_BAR_OFFSET_X,
                                  0 + HEALTH_BAR_OFFSET_Y,
                                  HEALTH_BAR_BG_WIDTH * HEALTH_BAR_SCALE,
                                  HEALTH_BAR_BG_HEIGHT * HEALTH_BAR_SCALE};
  SDL_SetRenderDrawColor(renderer, 31, 32, 34, 255);
  SDL_RenderFillRect(renderer, &healthBarBackground);

  SDL_Rect healthBar = {11 + HEALTH_BAR_OFFSET_X, 1 + HEALTH_BAR_OFFSET_Y,
                        length * HEALTH_BAR_SCALE,
                        HEALTH_BAR_HEIGHT * HEALTH_BAR_SCALE};
  SDL_SetRenderDrawColor(renderer, 5, 195, 221, 100);
  SDL_RenderFillRect(renderer, &healthBar);

  SDL_Color color = {255, 255, 255, 255};

  char text[256];
  char levelStr[50];
  snprintf(levelStr, sizeof(levelStr), "%d", player->level);
  snprintf(text, sizeof(text), "%s %s", HEALTH_BAR_TEXT, levelStr);
  SDL_Surface *surface =
      TTF_RenderText_Solid(getResourcesInstance()->font, text, color);
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

  SDL_Rect textRect = {12 + HEALTH_BAR_OFFSET_X, 1 + HEALTH_BAR_OFFSET_Y,
                       30 * HEALTH_BAR_SCALE,
                       HEALTH_BAR_HEIGHT * HEALTH_BAR_SCALE};

  SDL_RenderCopy(renderer, texture, NULL, &textRect);

  SDL_FreeSurface(surface);
  SDL_DestroyTexture(texture);

  // Render player's position next to the health bar
  char positionText[50];
  sprintf(positionText, "X: %d Y: %d", (int)player->base.movement.position.x,
          (int)player->base.movement.position.y);
  surface =
      TTF_RenderText_Solid(getResourcesInstance()->font, positionText, color);
  texture = SDL_CreateTextureFromSurface(renderer, surface);

  SDL_Rect positionRect = {12 + HEALTH_BAR_OFFSET_X + 40 * HEALTH_BAR_SCALE,
                           1 + HEALTH_BAR_OFFSET_Y, 100, 20};
  SDL_RenderCopy(renderer, texture, NULL, &positionRect);

  SDL_FreeSurface(surface);
  SDL_DestroyTexture(texture);
}

void renderEnemyCount(SDL_Renderer *renderer, Vector *enemies) {
  int aliveEnemies = 0;
  for (size_t i = 0; i < enemies->size; i++) {
    Enemy *enemy = (Enemy *)enemies->items[i];
    if (enemyIsAlive(enemy)) {
      aliveEnemies++;
    }
  }

  char enemyCountText[50];
  sprintf(enemyCountText, "Enemies: %d/%zu", aliveEnemies, enemies->size);

  SDL_Color color = {255, 255, 255, 255};
  SDL_Surface *surface =
      TTF_RenderText_Solid(getResourcesInstance()->font, enemyCountText, color);
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

  int textWidth = 100;
  int textHeight = 20;

  SDL_Rect textRect = {GAME_WIDTH - textWidth - 10, 10, textWidth, textHeight};
  SDL_RenderCopy(renderer, texture, NULL, &textRect);

  SDL_FreeSurface(surface);
  SDL_DestroyTexture(texture);
}

void centerCameraOnPlayer(Camera *camera, Humanoid *player, Map *map) {
  camera->x = player->movement.position.x - camera->width / 2;
  camera->y = player->movement.position.y - camera->height / 2;

  camera->x = (camera->x < 0) ? 0 : camera->x;
  camera->x = (camera->x > map->width - camera->width)
                  ? map->width - camera->width
                  : camera->x;
  camera->y = (camera->y < 0) ? 0 : camera->y;
  camera->y = (camera->y > map->height - camera->height)
                  ? map->height - camera->height
                  : camera->y;
}

void renderPlatforms(SDL_Renderer *renderer, Map *map, Camera *camera,
                     Humanoid *player) {
  for (size_t i = 0; i < map->platformCount; ++i) {
    if (abs((int)(map->platforms[i].x - player->movement.position.x)) <= 2000) {
      SDL_Rect rect = {map->platforms[i].x - camera->x,
                       map->platforms[i].y - camera->y, map->platforms[i].width,
                       map->platforms[i].height};

      if (SDL_RenderCopy(renderer, getResourcesInstance()->platformTexture,
                         NULL, &rect) != 0) {
        logError("SDL could not render the platform texture");
      }
    }
  }
}

void renderGameOverOverlay(SDL_Renderer *renderer) {
  const char *gameOverText = "Game Over";
  SDL_Color textColor = {255, 255, 255, 255}; // White color for Game Over text
  SDL_Color backgroundColor = {0, 0, 0,
                               128}; // Semi-transparent black background

  // Load a larger font size
  TTF_Font *largeFont =
      TTF_OpenFont(FONT_PATH, 72); // Adjust the path as needed
  if (!largeFont) {
    logError("Unable to load large font");
    return;
  }

  // Create surface and texture for the text
  SDL_Surface *textSurface =
      TTF_RenderText_Solid(largeFont, gameOverText, textColor);
  if (!textSurface) {
    logError("Unable to create text surface");
    TTF_CloseFont(largeFont);
    return;
  }

  SDL_Texture *textTexture =
      SDL_CreateTextureFromSurface(renderer, textSurface);
  if (!textTexture) {
    logError("Unable to create text texture");
    SDL_FreeSurface(textSurface);
    TTF_CloseFont(largeFont);
    return;
  }

  int textWidth = textSurface->w;
  int textHeight = textSurface->h;

  SDL_FreeSurface(textSurface);
  TTF_CloseFont(largeFont);

  // Set up the background rectangle
  SDL_Rect backgroundRect = {(GAME_WIDTH - textWidth) / 2 - 20,
                             (GAME_HEIGHT - textHeight) / 2 - 20,
                             textWidth + 40, textHeight + 40};

  // Render the background
  SDL_SetRenderDrawColor(renderer, backgroundColor.r, backgroundColor.g,
                         backgroundColor.b, backgroundColor.a);
  SDL_RenderFillRect(renderer, &backgroundRect);

  // Set up the text rectangle
  SDL_Rect textRect = {(GAME_WIDTH - textWidth) / 2,
                       (GAME_HEIGHT - textHeight) / 2, textWidth, textHeight};

  // Render the text
  SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

  SDL_DestroyTexture(textTexture);
}

void render(SDL_Renderer *renderer, Map *map, Player *player, Camera *camera,
            Vector *enemies) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);

  renderMap(renderer, map);
  renderPlatforms(renderer, map, camera, &player->base);
  renderHumanoid(renderer, &player->base, camera);

  for (size_t i = 0; i < enemies->size; i++) {
    Enemy *enemy = (Enemy *)enemies->items[i];
    if (abs((int)(enemy->base.movement.position.x -
                  player->base.movement.position.x)) <= 2000) {
      renderHumanoid(renderer, &enemy->base, camera);
    }
  }

  renderHealthBar(renderer, player);
  renderEnemyCount(renderer, enemies);

  // Check if player is alive, if not render GameOver overlay
  if (!playerIsAlive(player)) {
    renderGameOverOverlay(renderer);
  }

  SDL_RenderPresent(renderer);
}
