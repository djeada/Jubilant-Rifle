#include "utils.h"

void loadTexture(const char *file, SDL_Surface **surface) {
  // try loading an image, if failed, print error and return NULL

  *surface = IMG_Load(file);
  if (!*surface) {
    printf("IMG_Load: %s ", IMG_GetError());
    exit(1);
  }
}