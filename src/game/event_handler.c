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
      case SDLK_a:
        playerSetHorizontalVelocity(player, -200);
        break;
      case SDLK_RIGHT:
      case SDLK_d:
        playerSetHorizontalVelocity(player, 200);
        break;
      case SDLK_UP:
      case SDLK_w:
        playerSetVerticalVelocity(player, -200);
        /* Check if on ladder for climbing */
        playerStartClimbing(player);
        break;
      case SDLK_DOWN:
      case SDLK_s:
        playerSetVerticalVelocity(player, 200);
        break;
      case SDLK_SPACE:
      case SDLK_x:
        playerShoot(player, bulletPool);
        break;
      case SDLK_z:
      case SDLK_LCTRL:
        /* Melee attack */
        playerMeleeAttack(player);
        break;
      default:
        break;
      }
    }
    if (e->type == SDL_KEYUP) {
      switch (e->key.keysym.sym) {
      case SDLK_LEFT:
      case SDLK_RIGHT:
      case SDLK_a:
      case SDLK_d:
        playerStopHorizontal(player);
        break;
      case SDLK_UP:
      case SDLK_DOWN:
      case SDLK_w:
      case SDLK_s:
        playerStopVertical(player);
        playerStopClimbing(player);
        break;
      default:
        break;
      }
    }
  }
}

void handleGameEventsExtended(SDL_Event *e, Player *player, GameState *gameState,
                               bool *gameRunning, BulletPool *bulletPool,
                               GrenadePool *grenadePool) {
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
      case SDLK_a:
        playerSetHorizontalVelocity(player, -200);
        break;
      case SDLK_RIGHT:
      case SDLK_d:
        playerSetHorizontalVelocity(player, 200);
        break;
      case SDLK_UP:
      case SDLK_w:
        playerSetVerticalVelocity(player, -200);
        playerStartClimbing(player);
        break;
      case SDLK_DOWN:
      case SDLK_s:
        playerSetVerticalVelocity(player, 200);
        break;
      case SDLK_SPACE:
      case SDLK_x:
        playerShoot(player, bulletPool);
        break;
      case SDLK_z:
      case SDLK_LCTRL:
        /* Melee attack */
        playerMeleeAttack(player);
        break;
      case SDLK_g:
        /* Throw grenade */
        if (grenadePool && playerUseGrenade(player)) {
          float throwX = player->base.pos.x;
          float throwY = player->base.pos.y;
          float targetX = throwX + (player->base.direction == DIRECTION_RIGHT ? 200 : -200);
          float targetY = throwY;
          grenadePoolThrow(grenadePool, GRENADE_SOURCE_PLAYER, 
                           throwX, throwY, targetX, targetY);
        }
        break;
      default:
        break;
      }
    }
    if (e->type == SDL_KEYUP) {
      switch (e->key.keysym.sym) {
      case SDLK_LEFT:
      case SDLK_RIGHT:
      case SDLK_a:
      case SDLK_d:
        playerStopHorizontal(player);
        break;
      case SDLK_UP:
      case SDLK_DOWN:
      case SDLK_w:
      case SDLK_s:
        playerStopVertical(player);
        playerStopClimbing(player);
        break;
      default:
        break;
      }
    }
  }
}

