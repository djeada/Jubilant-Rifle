#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct {
  float x;
  float y;
} Point;

void loadSurface(const char *file, SDL_Surface **surface);
void loadTexture(const char *file, SDL_Renderer *renderer,
                 SDL_Texture **texture);
void pointConstructor(Point *point, float x, float y);
void pointCopyConstructor(Point *destination, Point *source);
Point createPoint(float x, float y);