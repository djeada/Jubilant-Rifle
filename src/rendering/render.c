#include "rendering/render.h"
#include "utils/utils.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

#include "entities/bullet.h"
#include "utils/resources.h"

void setRenderLogicalSize(SDL_Renderer *renderer, int windowWidth,
                          int windowHeight) {
  int gameWidth = 1920;  // Your game's designed width
  int gameHeight = 1080; // Your game's designed height

  // Determine the game's aspect ratio
  float gameAspectRatio = (float)gameWidth / (float)gameHeight;

  // Calculate the new window's aspect ratio
  float windowAspectRatio = (float)windowWidth / (float)windowHeight;

  // Declare variables for the renderer's logical size
  int rendererWidth, rendererHeight;

  if (windowAspectRatio < gameAspectRatio) {
    // Window is narrower than the game's aspect ratio
    rendererWidth = windowWidth;
    rendererHeight = (int)(windowWidth / gameAspectRatio);
  } else {
    // Window is as wide or wider than the game's aspect ratio
    rendererHeight = windowHeight;
    rendererWidth = (int)(windowHeight * gameAspectRatio);
  }

  // Set the logical size of the renderer
  SDL_RenderSetLogicalSize(renderer, rendererWidth, rendererHeight);
}

void renderMap(SDL_Renderer *renderer, Map *map) {
  SDL_Texture *bgTexture = NULL;
  SDL_Surface *bgSurface = IMG_Load(map->backgroundImage);

  if (bgSurface == NULL) {
    logError("Unable to load image");
    // More robust error handling
    goto cleanup;
  }

  bgTexture = SDL_CreateTextureFromSurface(renderer, bgSurface);
  if (bgTexture == NULL) {
    logError("Unable to create texture from surface");
    goto cleanup;
  }

  SDL_RenderCopy(renderer, bgTexture, NULL,
                 NULL); // Error checking could be added here

cleanup:
  if (bgSurface != NULL)
    SDL_FreeSurface(bgSurface);
  if (bgTexture != NULL)
    SDL_DestroyTexture(
        bgTexture); // We should not destroy texture after each render.
}

void renderPlayer(SDL_Renderer *renderer, Humanoid *player, Camera *camera) {
  if (!player->isAlive || !player->animation.isVisible) {
    return;
  }
  int spriteWidth = 40;   // Width of a single sprite frame
  int spriteHeight = 100; // Height of a single sprite frame
  int spritesPerRow = 6;  // The number of sprites per row in your sprite sheet

  // Calculate the x and y position on the sprite sheet
  int sheetX =
      (player->animation.currentSpriteIndex % spritesPerRow) * spriteWidth;
  int sheetY =
      (player->animation.currentSpriteIndex / spritesPerRow) * spriteHeight;

  // Define the source rectangle (part of the sprite sheet you want to draw)
  SDL_Rect sourceRect = {sheetX, sheetY, spriteWidth, spriteHeight};

  // Define the destination rectangle (where on the window you want to draw)
  SDL_Rect destinationRect = {player->movement.position.x - camera->x,
                              player->movement.position.y - camera->y,
                              spriteWidth, spriteHeight};

  // Set the flip status based on the player's direction
  SDL_RendererFlip flip =
      player->animation.isFacingLeft ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

  // Render the current sprite frame with flipping
  if (SDL_RenderCopyEx(renderer, player->texture, &sourceRect, &destinationRect,
                       0.0, NULL, flip) != 0) {
    logError("Unable to render player");
  }

  // Render bullets
  for (size_t i = 0; i < player->bulletManager.bullets.size; i++) {
    Bullet *bullet = (Bullet *)player->bulletManager.bullets.items[i];
    if (bullet && bullet->animation.isVisible) {
      int bulletSpriteWidth =
          20; // Set the appropriate dimensions for your bullet sprites
      int bulletSpriteHeight =
          20; // Set the appropriate dimensions for your bullet sprites

      // Calculate the position on the sprite sheet for the bullet
      int bulletSheetX =
          (bullet->animation.currentSpriteIndex % spritesPerRow) *
          bulletSpriteWidth;
      int bulletSheetY =
          (bullet->animation.currentSpriteIndex / spritesPerRow) *
          bulletSpriteHeight;

      SDL_Rect bulletSourceRect = {bulletSheetX, bulletSheetY,
                                   bulletSpriteWidth, bulletSpriteHeight};

      SDL_Rect bulletDestinationRect = {bullet->movement.position.x - camera->x,
                                        bullet->movement.position.y - camera->y,
                                        bulletSpriteWidth, bulletSpriteHeight};

      // Render the bullet sprite
      if (SDL_RenderCopyEx(renderer, bullet->texture, &bulletSourceRect,
                           &bulletDestinationRect, 0.0, NULL,
                           SDL_FLIP_NONE) != 0) {
        logError("Unable to render bullet");
      }
    }
  }
}

void renderHealthBar(SDL_Renderer *renderer, unsigned int length) {
  // Move down and to the right (e.g., by 20 pixels each), and make 2x bigger
  int offsetX = 20;
  int offsetY = 20;
  int scaleFactor = 2;

  SDL_Rect healthBarBackground = {
      10 + offsetX,      // x position
      0 + offsetY,       // y position
      102 * scaleFactor, // width
      12 * scaleFactor   // height
  };
  SDL_SetRenderDrawColor(renderer, 31, 32, 34, 255);
  SDL_RenderFillRect(renderer, &healthBarBackground);

  SDL_Rect healthBar = {
      11 + offsetX,         // x position
      1 + offsetY,          // y position
      length * scaleFactor, // width (the length should be scaled if it
                            // represents the actual health bar's length)
      10 * scaleFactor      // height
  };
  SDL_SetRenderDrawColor(renderer, 5, 195, 221, 100);
  SDL_RenderFillRect(renderer, &healthBar);

  // Assuming `font` has been defined and set up somewhere above in the code
  SDL_Color color = {255, 255, 255, 255};
  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

  SDL_Surface *surface =
      TTF_RenderText_Solid(getResourcesInstance()->font, "Level 1 ", color);
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

  // Text position and size also need to be adjusted
  SDL_Rect textRect = {
      12 + offsetX,     // x position
      1 + offsetY,      // y position
      30 * scaleFactor, // assuming the width is dynamic based on text length
      10 * scaleFactor  // height
  };

  SDL_RenderCopy(renderer, texture, NULL, &textRect);

  // Clean up
  SDL_FreeSurface(surface);
  SDL_DestroyTexture(texture);
}

void centerCameraOnPlayer(Camera *camera, Humanoid *player) {

  const int mapWidth = 2500;
  const int mapHeight = 1000;

  camera->x = player->movement.position.x - camera->width / 2;
  camera->y = player->movement.position.y - camera->height / 2;

  // Horizontal bounds
  camera->x = (camera->x < 0) ? 0 : camera->x;
  camera->x = (camera->x > mapWidth - camera->width) ? mapWidth - camera->width
                                                     : camera->x;

  // Vertical bounds
  camera->y = (camera->y < 0) ? 0 : camera->y;
  camera->y = (camera->y > mapHeight - camera->height)
                  ? mapHeight - camera->height
                  : camera->y;
}

void renderPlatforms(SDL_Renderer *renderer, Map *map, Camera *camera) {
  // Assuming platformTexture is a valid SDL_Texture* that has been created
  // elsewhere in the code

  for (size_t i = 0; i < map->platformCount; ++i) {
    SDL_Rect rect = {map->platforms[i].x - camera->x,
                     map->platforms[i].y - camera->y, map->platforms[i].width,
                     map->platforms[i].height};

    // Render the platform texture
    if (SDL_RenderCopy(renderer, getResourcesInstance()->platformTexture, NULL,
                       &rect) != 0) {
      logError("SDL could not render the platform texture");
    }
  }
}

void render(SDL_Renderer *renderer, Map *map, Humanoid *player, Camera *camera,
            Vector *enemies) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0,
                         SDL_ALPHA_OPAQUE); // Clear with black background
  SDL_RenderClear(renderer);

  renderMap(renderer, map);
  renderPlatforms(renderer, map, camera);
  renderPlayer(renderer, player, camera);
  for (size_t i = 0; i < enemies->size; i++) {
    Humanoid *enemy = (Humanoid *)enemies->items[i];
    renderPlayer(renderer, enemy, camera);
  }
  renderHealthBar(renderer, 100);
  SDL_RenderPresent(renderer); // Update screen
}
