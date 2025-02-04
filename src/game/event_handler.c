#include "game/event_handler.h"

void handleGameEvents(SDL_Event *e, Player *player, GameState *gameState,
                      bool *gameRunning, BulletPool *bulletPool) {
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
      switch (e->key.keysym.sym) {
      case SDLK_LEFT:
        playerSetHorizontalVelocity(player, -200);
        break;
      case SDLK_RIGHT:
        playerSetHorizontalVelocity(player, 200);
        break;
      case SDLK_UP:
        playerSetVerticalVelocity(player, -200);
        break;
      case SDLK_DOWN:
        playerSetVerticalVelocity(player, 200);
        break;
      case SDLK_SPACE:
        playerShoot(player, bulletPool);
        break;
      }
    }
    if (e->type == SDL_KEYUP) {
      switch (e->key.keysym.sym) {
      case SDLK_LEFT:
      case SDLK_RIGHT:
        playerStopHorizontal(player);
        break;
      case SDLK_UP:
      case SDLK_DOWN:
        playerStopVertical(player);
        break;
      }
    }
  }
}

