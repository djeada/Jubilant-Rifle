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
      if (e->key.keysym.sym == SDLK_LEFT)
        player->base.vel.x = -200;
      if (e->key.keysym.sym == SDLK_RIGHT)
        player->base.vel.x = 200;
      if (e->key.keysym.sym == SDLK_UP)
        player->base.vel.y = -200;
      if (e->key.keysym.sym == SDLK_DOWN)
        player->base.vel.y = 200;
      if (e->key.keysym.sym == SDLK_SPACE)
        bulletPoolSpawn(bulletPool, BULLET_SOURCE_PLAYER,
                        player->base.pos.x + 20, player->base.pos.y, 300, 0);
    }
    if (e->type == SDL_KEYUP) {
      if (e->key.keysym.sym == SDLK_LEFT || e->key.keysym.sym == SDLK_RIGHT)
        player->base.vel.x = 0;
      if (e->key.keysym.sym == SDLK_UP || e->key.keysym.sym == SDLK_DOWN)
        player->base.vel.y = 0;
    }
  }
}

