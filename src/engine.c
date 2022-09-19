#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "consts.h"
#include "engine.h"
#include "map.h"

int globalTime = 0;

bool processEvents(SDL_Window *window, Humanoid *man, Bullet **bullets) {
  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_WINDOWEVENT_CLOSE: {
      if (window) {
        SDL_DestroyWindow(window);
        window = NULL;
        return false;
      }
      
    } break;
    case SDL_KEYDOWN: {
      switch (event.key.keysym.sym) {
      case SDLK_ESCAPE:
        return false;
      }
    } break;
    case SDL_QUIT:
      return false;
    }
  }

  const unsigned char *state = SDL_GetKeyboardState(NULL);
  if (!man->shooting) {
    if (state[SDL_SCANCODE_LEFT]) {
      moveLeft(man);

      if (globalTime % 6 == 0) {
        incrementSprite(man);
      }
    } else if (state[SDL_SCANCODE_RIGHT]) {
      moveRight(man);

      if (globalTime % 6 == 0) {
        incrementSprite(man);
      }
    } else {
      stop(man);
    }

      if (state[SDL_SCANCODE_UP]) {
    jump(man);
  }
  }

  if (!man->walking) {
    if (state[SDL_SCANCODE_SPACE]) // && !man->dy)
    {
      if (globalTime % 6 == 0) {
        shoot(man, bullets);
      }

    } else {
      man->currentSprite = 4;
      man->shooting = 0;
    }
  }

  return true;
}

void doRender(SDL_Renderer *renderer, Humanoid *man, Humanoid *enemy,
              Bullet **bullets, SDL_Texture *backgroundTexture,
              SDL_Texture *bulletTexture) {

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);   // set the drawing color to black
  SDL_RenderClear(renderer);

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);   // set the drawing color to white
  SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

  // warrior
  if (man->visible) {
    SDL_Rect srcRect = {40 * man->currentSprite, 0, 40, 50};
    SDL_Rect rect = {man->x, man->y, 40, 50};
    SDL_RenderCopyEx(renderer, man->sheetTexture, &srcRect, &rect, 0, NULL,
                     man->facingLeft);
  }

  // enemy
  if (enemy->visible) {
    SDL_Rect eSrcRect = {40 * enemy->currentSprite, 0, 40, 50};
    SDL_Rect eRect = {enemy->x, enemy->y, 40, 50};
    SDL_RenderCopyEx(renderer, enemy->sheetTexture, &eSrcRect, &eRect, 0, NULL,
                     enemy->facingLeft);
  }

  for (int i = 0; i < MAX_BULLETS; i++)
    if (bullets[i]) {
      SDL_Rect rect = {bullets[i]->x, bullets[i]->y, 8, 8};
      SDL_RenderCopy(renderer, bulletTexture, NULL, &rect);
    }

  SDL_RenderPresent(renderer);
}

void updateLogic(Humanoid *man, Humanoid *enemy, Bullet **bullets) {
  man->y += man->dy;
  man->dy += DELTA_Y;

  int groundLevel = coordinatesToGroundLevel(man->x, man->y);

  if (man->y > groundLevel) {
    man->y = groundLevel;
    man->dy = 0;
  }

  for (int i = 0; i < MAX_BULLETS; i++)
    if (bullets[i]) {
      bullets[i]->x += bullets[i]->dx;

      // simple coll. detection
      if (bullets[i]->x > enemy->x && bullets[i]->x < enemy->x + 40 &&
          bullets[i]->y > enemy->y && bullets[i]->y < enemy->y + 50) {
        die(enemy);
      }

      if (bullets[i]->x < -1000 || bullets[i]->x > 1000)
        removeBullet(i, bullets);
    }

  if (globalTime % 15 == 0) {

    if (!enemy->alive && enemy->visible) {

      enemy->visible = false;
    }
  }

  globalTime++;
}
