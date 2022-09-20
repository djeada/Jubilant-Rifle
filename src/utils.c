#include "utils.h"

void loadSurface(const char *file, SDL_Surface **surface) {
  // try loading an image, if failed, print error and return NULL

  *surface = IMG_Load(file);
  if (!*surface) {
    printf("IMG_Load: %s ", IMG_GetError());
    exit(1);
  }
}

void loadTexture(const char *file, SDL_Renderer *renderer,
                 SDL_Texture **texture) {
  SDL_Surface *sheet;
  loadSurface(file, &sheet);

  *texture = SDL_CreateTextureFromSurface(renderer, sheet);
  SDL_FreeSurface(sheet);
}