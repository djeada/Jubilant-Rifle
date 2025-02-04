#ifndef TEXT_MANAGER_H
#define TEXT_MANAGER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

typedef struct {
  SDL_Texture *playerTex;
  SDL_Texture *enemyTex;
  SDL_Texture *bulletTex;
  SDL_Texture *backgroundTex;
  TTF_Font *font;
} TextureManager;

TextureManager
initTextureManager(SDL_Renderer *renderer, const char *playerTexturePath,
                   const char *enemyTexturePath, const char *bulletTexturePath,
                   const char *backgroundTexturePath, const char *fontPath);
void destroyTextureManager(TextureManager *tm);

#endif // TEXT_MANAGER_H

