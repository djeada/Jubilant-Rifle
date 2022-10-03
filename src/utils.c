#include <math.h>
#include "utils.h"

void loadSurface(const char *file, SDL_Surface **surface) {
  // try loading an image, if failed, print error and exit
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

void pointConstructor(Point *point, float x, float y) {
  point->x = x;
  point->y = y;
}

void pointCopyConstructor(Point *destination, Point *source) {
  pointConstructor(destination, source->x, source->y);
}

Point createPoint(float x, float y) {
  Point point;
  pointConstructor(&point, x, y);
  return point;
}

unsigned int numberFromString(const char *line) {
  /* returns all digits from a line, while ingoring all other characters */
  unsigned int number = 0;
  unsigned int i = 0;
  while (line[i] != '\0') {
    if (line[i] >= '0' && line[i] <= '9') {
      number = number * 10 + (line[i] - '0');
    }
    i++;
  }
  return number;
}

bool arePointsInProximity(Point *point1, Point *point2, float proximity) {
  float distance = sqrt(pow(point1->x - point2->x, 2) +
                        pow(point1->y - point2->y, 2));
  return distance < proximity;
}
