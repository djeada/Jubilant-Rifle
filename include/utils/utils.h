#ifndef UTILS_H
#define UTILS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

void loadSurface(const char *file, SDL_Surface **surface);
bool loadTexture(SDL_Renderer *renderer, const char *path,
                 SDL_Texture **texture);
unsigned int numberFromString(const char *line);
void logError(const char *msg);
char *customStrdup(const char *str);

#endif
