#include "game/event_handler.h"
#include "utils/consts.h"

bool ProcessWindowEvent(const SDL_Event *event, SDL_Window **window) {
  if (event->type == SDL_WINDOWEVENT &&
      event->window.event == SDL_WINDOWEVENT_CLOSE) {
    if (*window) {
      SDL_DestroyWindow(*window);
      *window = NULL;
      return false;
    }
  }
  return true;
}

void ToggleFullscreen(SDL_Window *window) {
  Uint32 fullscreenFlag = SDL_WINDOW_FULLSCREEN_DESKTOP;
  Uint32 isFullscreen = SDL_GetWindowFlags(window) & fullscreenFlag;
  SDL_SetWindowFullscreen(window, isFullscreen ? 0 : fullscreenFlag);
  if (!isFullscreen) {
    SDL_SetWindowSize(window, WINDOWED_MODE_WIDTH, WINDOWED_MODE_HEIGHT);
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED,
                          SDL_WINDOWPOS_CENTERED);
  }
}

bool ProcessKeyEvent(const SDL_Event *event, SDL_Window *window) {
  if (event->type == SDL_KEYDOWN) {
    switch (event->key.keysym.sym) {
    case SDLK_ESCAPE:
      return false;
    case SDLK_f:
      ToggleFullscreen(window);
      break;
    }
  } else if (event->type == SDL_QUIT) {
    return false;
  }
  return true;
}

void HandlePlayerState(const Uint8 *state, Player *player) {
  if (state[SDL_SCANCODE_UP]) {
    player->base.movement.velocity.y = -JUMP_SPEED;
  }
  if (state[SDL_SCANCODE_LEFT]) {
    player->base.movement.velocity.x = -MOVE_SPEED;
    player->base.animation.isFacingLeft = true;
  }
  if (state[SDL_SCANCODE_RIGHT]) {
    player->base.movement.velocity.x = MOVE_SPEED;
    player->base.animation.isFacingLeft = false;
  }
  if (state[SDL_SCANCODE_SPACE]) {
    humanoidShoot(&player->base);
  }
}

bool processEvents(SDL_Window *window, Player *player) {
  SDL_Event event;
  const Uint8 *state = SDL_GetKeyboardState(NULL);

  while (SDL_PollEvent(&event)) {
    if (!ProcessWindowEvent(&event, &window) ||
        !ProcessKeyEvent(&event, window)) {
      return false;
    }
  }

  if (player && playerIsAlive(player)) {
    HandlePlayerState(state, player);
  }

  return true;
}
