#include "humanoid.h"
#include "vector.h"
#include <stdbool.h>

bool processEvents(SDL_Window *window, Humanoid *man, Vector *bullets);
void doRender(SDL_Renderer *renderer, Humanoid *man, Humanoid *enemy,
              Vector *bullets, SDL_Texture *backgroundTexture,
              SDL_Texture *bulletTexture);
void updateLogic(Humanoid *man, Humanoid *enemy, Vector *bullets);
