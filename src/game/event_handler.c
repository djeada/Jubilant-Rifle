#include "game/event_handler.h"

void handleGameEvents(SDL_Event *e, Entity *player, GameState *gameState, bool *gameRunning) {
    while (SDL_PollEvent(e)) {
        if (e->type == SDL_QUIT) {
            *gameState = STATE_EXIT;
            *gameRunning = false;
        }
        if (e->type == SDL_KEYDOWN) {
            if (e->key.keysym.sym == SDLK_ESCAPE) {
                *gameState = STATE_MENU;
                *gameRunning = false;
            }
            if (e->key.keysym.sym == SDLK_LEFT)
                player->vel.x = -200;
            if (e->key.keysym.sym == SDLK_RIGHT)
                player->vel.x = 200;
            if (e->key.keysym.sym == SDLK_UP)
                player->vel.y = -200;
            if (e->key.keysym.sym == SDLK_DOWN)
                player->vel.y = 200;
        }
        if (e->type == SDL_KEYUP) {
            if (e->key.keysym.sym == SDLK_LEFT || e->key.keysym.sym == SDLK_RIGHT)
                player->vel.x = 0;
            if (e->key.keysym.sym == SDLK_UP || e->key.keysym.sym == SDLK_DOWN)
                player->vel.y = 0;
        }
    }
}
