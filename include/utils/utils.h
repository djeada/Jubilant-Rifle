#ifndef UTILS_H
#define UTILS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

typedef struct {
  float x;
  float y;
} Point;

void loadSurface(const char *file, SDL_Surface **surface);
bool loadTexture(SDL_Renderer *renderer, const char *path,
                 SDL_Texture **texture);
unsigned int numberFromString(const char *line);
void pointConstructor(Point *point, float x, float y);
void pointCopyConstructor(Point *destination, Point *source);
Point createPoint(float x, float y);
bool arePointsInProximity(Point *point1, Point *point2, float proximity);
bool arePointsInOrder(Point *point1, Point *point2);
char *my_strdup(const char *str);
void LogSDLError(const char *msg);
#endif
