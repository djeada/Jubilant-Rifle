#include "rendering/render.h"
#include "entities/bullet.h"
#include "utils/resources.h"
#include "utils/utils.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

// Constants
#define GAME_WIDTH 1920
#define GAME_HEIGHT 1080
#define SPRITE_WIDTH 40
#define SPRITE_HEIGHT 100
#define SPRITES_PER_ROW 6
#define BULLET_SPRITE_WIDTH 20
#define BULLET_SPRITE_HEIGHT 20
#define HEALTH_BAR_OFFSET_X 20
#define HEALTH_BAR_OFFSET_Y 20
#define HEALTH_BAR_SCALE 2
#define HEALTH_BAR_BG_WIDTH 102
#define HEALTH_BAR_BG_HEIGHT 12
#define HEALTH_BAR_WIDTH 100
#define HEALTH_BAR_HEIGHT 10
#define HEALTH_BAR_TEXT "Level 1 "

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

void renderPlayer(SDL_Renderer *renderer, Humanoid *player, Camera *camera) {
  if (!player->isAlive || !player->animation.isVisible)
    return;

  int sheetX =
      (player->animation.currentSpriteIndex % SPRITES_PER_ROW) * SPRITE_WIDTH;
  int sheetY =
      (player->animation.currentSpriteIndex / SPRITES_PER_ROW) * SPRITE_HEIGHT;

  SDL_Rect sourceRect = {sheetX, sheetY, SPRITE_WIDTH, SPRITE_HEIGHT};
  SDL_Rect destinationRect = {player->movement.position.x - camera->x,
                              player->movement.position.y - camera->y,
                              SPRITE_WIDTH, SPRITE_HEIGHT};

  SDL_RendererFlip flip =
      player->animation.isFacingLeft ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

  if (SDL_RenderCopyEx(renderer, player->texture, &sourceRect, &destinationRect,
                       0.0, NULL, flip) != 0) {
    logError("Unable to render player");
  }

  renderBullets(renderer, &player->bulletManager, camera);
}

void renderHealthBar(SDL_Renderer *renderer, unsigned int length) {
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
  SDL_Surface *surface = TTF_RenderText_Solid(getResourcesInstance()->font,
                                              HEALTH_BAR_TEXT, color);
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

  SDL_Rect textRect = {12 + HEALTH_BAR_OFFSET_X, 1 + HEALTH_BAR_OFFSET_Y,
                       30 * HEALTH_BAR_SCALE,
                       HEALTH_BAR_HEIGHT * HEALTH_BAR_SCALE};

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

void renderPlatforms(SDL_Renderer *renderer, Map *map, Camera *camera) {
  for (size_t i = 0; i < map->platformCount; ++i) {
    SDL_Rect rect = {map->platforms[i].x - camera->x,
                     map->platforms[i].y - camera->y, map->platforms[i].width,
                     map->platforms[i].height};

    if (SDL_RenderCopy(renderer, getResourcesInstance()->platformTexture, NULL,
                       &rect) != 0) {
      logError("SDL could not render the platform texture");
    }
  }
}

void render(SDL_Renderer *renderer, Map *map, Humanoid *player, Camera *camera,
            Vector *enemies) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);

  renderMap(renderer, map);
  renderPlatforms(renderer, map, camera);
  renderPlayer(renderer, player, camera);

  for (size_t i = 0; i < enemies->size; i++) {
    Humanoid *enemy = (Humanoid *)enemies->items[i];
    renderPlayer(renderer, enemy, camera);
  }

  renderHealthBar(renderer, 100);
  SDL_RenderPresent(renderer);
}
