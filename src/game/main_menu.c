#include "game/main_menu.h"

void runMainMenu(SDL_Renderer *renderer, GameState *state) {
    SDL_Event event;
    bool menuRunning = true;

    while (menuRunning) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                *state = STATE_EXIT;
                return;
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_RETURN) {
                    *state = STATE_GAME;
                    return;
                }
            }
        }
        
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
        SDL_RenderClear(renderer);
        
        /* Optionally render menu text/buttons here */
        
        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }
}
