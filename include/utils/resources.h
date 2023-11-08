#ifndef RESOURCES_H
#define RESOURCES_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
// A struct to hold all our game resources
typedef struct Resources {
  SDL_Texture *backgroundTexture;
  SDL_Texture *bulletTexture;
  SDL_Texture *playerTexture;
  SDL_Texture *enemyTexture;
  TTF_Font *font;
} Resources;
#endif // RESOURCES_H