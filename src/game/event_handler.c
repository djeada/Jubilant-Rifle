#include "game/event_handler.h"

static const int DefaultWindowWidth = 1280;
static const int DefaultWindowHeight = 1080;
static const int MoveSpeed = 5;

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
    SDL_SetWindowSize(window, DefaultWindowWidth, DefaultWindowHeight);
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

void HandlePlayerMovement(const Uint8 *state, Humanoid *player) {
  if (state[SDL_SCANCODE_UP]) {
    player->movement.velocity.y = -200;
  }
  if (state[SDL_SCANCODE_LEFT]) {
    player->movement.velocity.x = -MoveSpeed;
    player->animation.isFacingLeft =
        true; // Update the direction the player is facing
  }
  if (state[SDL_SCANCODE_RIGHT]) {
    player->movement.velocity.x = MoveSpeed;
    player->animation.isFacingLeft =
        false; // Update the direction the player is facing
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
    HandlePlayerMovement(state, player);
  }

  return true;
}
