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

void HandlePlayerState(const Uint8 *state, Humanoid *player) {
  if (state[SDL_SCANCODE_UP]) {
    player->movement.velocity.y = -JUMP_SPEED;
  }
  if (state[SDL_SCANCODE_LEFT]) {
    player->movement.velocity.x = -MOVE_SPEED;
    player->animation.isFacingLeft =
        true; // Update the direction the player is facing
  }
  if (state[SDL_SCANCODE_RIGHT]) {
    player->movement.velocity.x = MOVE_SPEED;
    player->animation.isFacingLeft =
        false; // Update the direction the player is facing
  }
  if (state[SDL_SCANCODE_SPACE] || state[SDLK_SPACE]) { // Handle the space key
    humanoidShoot(player);
  }
}

bool processEvents(SDL_Window *window, Humanoid *player) {
  SDL_Event event;
  const Uint8 *state = SDL_GetKeyboardState(NULL);

  while (SDL_PollEvent(&event)) {
    if (!ProcessWindowEvent(&event, &window) ||
        !ProcessKeyEvent(&event, window)) {
      return false;
    }
  }

  // Handle player movement if the player is not null
  if (player) {
    HandlePlayerState(state, player);
  }

  return true;
}
