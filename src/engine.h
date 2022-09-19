#include "humanoid.h"
#include <stdbool.h>

bool processEvents(SDL_Window *window, Humanoid *man, Bullet **bullets);
void doRender(SDL_Renderer *renderer, Humanoid *man, Humanoid *enemy,
              Bullet **bullets, SDL_Texture *backgroundTexture,
              SDL_Texture *bulletTexture);
void updateLogic(Humanoid *man, Humanoid *enemy, Bullet **bullets);
