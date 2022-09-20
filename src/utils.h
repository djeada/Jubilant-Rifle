#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

void loadSurface(const char *file, SDL_Surface **surface);
void loadTexture(const char *file, SDL_Renderer *renderer,
                 SDL_Texture **texture);
