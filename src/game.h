
#include "humanoid.h"
#include "vector.h"
#include <stdbool.h>

extern int globalTime;

bool processEvents(SDL_Window *window, Humanoid *man, Vector *bullets);
void render(SDL_Renderer *renderer, Humanoid *man, Vector *enemies,
            Vector *bullets, SDL_Texture *backgroundTexture,
            SDL_Texture *bulletTexture);

void updateLogic(Humanoid *man, Vector *enemies, Vector *bullets);
void run_game();