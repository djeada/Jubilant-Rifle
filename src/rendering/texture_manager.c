#include "rendering/texture_manager.h"
#include "utils/consts.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>

static SDL_Texture *createFallbackTexture(SDL_Renderer *renderer,
                                          SDL_Color color, int w, int h) {
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
static SDL_Texture *loadTextureOrFallback(SDL_Renderer *renderer,
                                          const char *path,
                                          SDL_Color fallbackColor,
                                          int fallbackW, int fallbackH) {
  SDL_Texture *texture = IMG_LoadTexture(renderer, path);
  if (!texture) {
    fprintf(stderr, "Failed to load texture from %s: %s\n", path,
            IMG_GetError());
    texture =
        createFallbackTexture(renderer, fallbackColor, fallbackW, fallbackH);
  }
  return texture;
}

TextureManager
initTextureManager(SDL_Renderer *renderer, const char *playerTexturePath,
                   const char *enemyTexturePath, const char *bulletTexturePath,
                   const char *backgroundTexturePath, const char *fontPath) {
  TextureManager tm;

  // Load player texture (fallback: blue rectangle)
  tm.playerTex = loadTextureOrFallback(renderer, playerTexturePath,
                                       (SDL_Color){0, 0, 255, 255},
                                       SPRITE_WIDTH, HUMANOID_FRAME_HEIGHT);

  // Load enemy texture (fallback: green rectangle)
  tm.enemyTex = loadTextureOrFallback(renderer, enemyTexturePath,
                                      (SDL_Color){0, 255, 0, 255}, SPRITE_WIDTH,
                                      HUMANOID_FRAME_HEIGHT);

  // Load bullet texture (fallback: red rectangle)
  tm.bulletTex = loadTextureOrFallback(
      renderer, bulletTexturePath, (SDL_Color){255, 0, 0, 255},
      BULLET_SPRITE_WIDTH, BULLET_SPRITE_HEIGHT);

  // Load background texture (fallback: white rectangle)
  tm.backgroundTex = loadTextureOrFallback(renderer, backgroundTexturePath,
                                           (SDL_Color){255, 255, 255, 255},
                                           GAME_WIDTH, GAME_HEIGHT);

  // Load font
  tm.font = TTF_OpenFont(fontPath, 28);
  if (!tm.font) {
    fprintf(stderr, "Failed to load font from %s: %s\n", fontPath,
            TTF_GetError());
  }

  return tm;
}

void destroyTextureManager(TextureManager *tm) {
  SDL_DestroyTexture(tm->playerTex);
  SDL_DestroyTexture(tm->enemyTex);
  SDL_DestroyTexture(tm->bulletTex);
  SDL_DestroyTexture(tm->backgroundTex);
  TTF_CloseFont(tm->font);
}

