#include "game/event_handler.h"

void handleGameEvents(SDL_Event *e, Entity *player, GameState *gameState,
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
        player->vel.x = -200;
      if (e->key.keysym.sym == SDLK_RIGHT)
        player->vel.x = 200;
      if (e->key.keysym.sym == SDLK_UP)
        player->vel.y = -200;
      if (e->key.keysym.sym == SDLK_DOWN)
        player->vel.y = 200;
      if (e->key.keysym.sym == SDLK_SPACE) {
        // Player shoots: spawn a bullet from the player’s position going
        // upward. Adjust the spawn offset so that the bullet appears from the
        // center/top of the player.
        bullet_pool_spawn(bulletPool, player->pos.x + 20, player->pos.y, 0,
                          -300);
      }
    }
    if (e->type == SDL_KEYUP) {
      if (e->key.keysym.sym == SDLK_LEFT || e->key.keysym.sym == SDLK_RIGHT)
        player->vel.x = 0;
      if (e->key.keysym.sym == SDLK_UP || e->key.keysym.sym == SDLK_DOWN)
        player->vel.y = 0;
    }
  }
}
