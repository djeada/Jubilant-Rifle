#include "rendering/texture_manager.h"
#include "utils/consts.h"
#include <SDL2/SDL_image.h>
#include <stdio.h>

// --- Helper: Create Fallback Texture ---
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

TextureManager initTextureManager(SDL_Renderer *renderer) {
  TextureManager tm;
  tm.playerTex = loadTextureOrFallback(renderer, PLAYER_TEXTURE_PATH,
                                       (SDL_Color){0, 0, 255, 255},
                                       SPRITE_WIDTH, HUMANOID_FRAME_HEIGHT);
  tm.enemyTex = loadTextureOrFallback(renderer, ENEMY_TEXTURE_PATH,
                                      (SDL_Color){0, 255, 0, 255}, SPRITE_WIDTH,
                                      HUMANOID_FRAME_HEIGHT);
  tm.bulletTex = loadTextureOrFallback(
      renderer, BULLET_TEXTURE_PATH, (SDL_Color){255, 0, 0, 255},
      BULLET_SPRITE_WIDTH, BULLET_SPRITE_HEIGHT);
  return tm;
}

void destroyTextureManager(TextureManager *tm) {
  SDL_DestroyTexture(tm->playerTex);
  SDL_DestroyTexture(tm->enemyTex);
  SDL_DestroyTexture(tm->bulletTex);
}
