#ifndef RESOURCES_H
#define RESOURCES_H

#include "utils/utils.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
// A struct to hold all our game resources
typedef struct Resources {
  SDL_Texture *backgroundTexture;
  SDL_Texture *bulletTexture;
  SDL_Texture *playerTexture;
  SDL_Texture *enemyTexture;
  SDL_Texture *platformTexture;
  TTF_Font *font;
} Resources;

// Function prototypes
Resources *getResourcesInstance();
bool initializeResourcesInstance(SDL_Renderer *renderer);
void freeResourcesInstance();

#endif // RESOURCES_H
