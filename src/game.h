
#include <stdbool.h>

#include "humanoid.h"
#include "vector.h"

extern int globalTime;

bool processEvents(SDL_Window *window, Humanoid *man, Vector *bullets);
void render(SDL_Renderer *renderer, Humanoid *player, Vector *enemies,
            Vector *bullets);
void updateLogic(Humanoid *man, Vector *enemies, Vector *bullets);
void run_game();
