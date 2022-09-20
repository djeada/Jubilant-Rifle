#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

#include "consts.h"
#include "engine.h"
#include "map.h"

int globalTime = 0;

bool processEvents(SDL_Window *window, Humanoid *man, Vector *bullets) {
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

void doRender(SDL_Renderer *renderer, Humanoid *man, Vector *enemies,
              Vector *bullets, SDL_Texture *backgroundTexture,
              SDL_Texture *bulletTexture) {

  SDL_SetRenderDrawColor(renderer, 0, 0, 0,
                         0); // set the drawing color to black
  SDL_RenderClear(renderer);

  SDL_SetRenderDrawColor(renderer, 255, 255, 255,
                         255); // set the drawing color to white
  SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

  // warrior
  if (man->visible) {
    SDL_Rect srcRect = {40 * man->currentSprite, 0, 40, 50};
    SDL_Rect rect = {man->x, man->y, 40, 50};
    SDL_RenderCopyEx(renderer, man->texture, &srcRect, &rect, 0, NULL,
                     man->facingLeft);
  }

  // enemy

  for (int i = 0; i < enemies->size; i++) {
    if (((Humanoid *)enemies->data[i])->visible) {
      SDL_Rect eSrcRect = {40 * ((Humanoid *)enemies->data[i])->currentSprite,
                           0, 40, 50};
      SDL_Rect eRect = {((Humanoid *)enemies->data[i])->x,
                        ((Humanoid *)enemies->data[i])->y, 40, 50};
      SDL_RenderCopyEx(renderer, ((Humanoid *)enemies->data[i])->texture,
                       &eSrcRect, &eRect, 0, NULL,
                       ((Humanoid *)enemies->data[i])->facingLeft);
    }
  }
  for (int i = 0; i < MAX_BULLETS; i++)
    if (bullets->data[i]) {
      SDL_Rect rect = {((Bullet *)bullets->data[i])->x,
                       ((Bullet *)bullets->data[i])->y, 8, 8};
      SDL_RenderCopy(renderer, bulletTexture, NULL, &rect);
    }

  SDL_RenderPresent(renderer);
}

void updateLogic(Humanoid *man, Vector *enemies, Vector *bullets) {
  man->y += man->dy;
  man->dy += DELTA_Y;

  int groundLevel = coordinatesToGroundLevel(man->x, man->y);

  if (man->y > groundLevel) {
    man->y = groundLevel;
    man->dy = 0;
  }

  for (int i = 0; i < bullets->size; i++) {

    ((Bullet *)bullets->data[i])->x += ((Bullet *)bullets->data[i])->dx;

    if (((Bullet *)bullets->data[i])->x < -1000 ||
        ((Bullet *)bullets->data[i])->x > 1000) {
      printf("%d \n", bullets->size);
      removeFromVector(bullets, i);
      printf("%d \n", bullets->size);

      continue;
    }

    for (int j = 0; j < enemies->size; j++) {

      if (((Bullet *)bullets->data[i])->x > ((Humanoid *)enemies->data[j])->x &&
          ((Bullet *)bullets->data[i])->x <
              ((Humanoid *)enemies->data[j])->x + 40 &&
          ((Bullet *)bullets->data[i])->y > ((Humanoid *)enemies->data[j])->y &&
          ((Bullet *)bullets->data[i])->y <
              ((Humanoid *)enemies->data[j])->y + 50) {
        die(((Humanoid *)enemies->data[j]));
      }

      if (globalTime % 15 == 0) {

        if (!((Humanoid *)enemies->data[j])->alive &&
            ((Humanoid *)enemies->data[j])->visible) {

          ((Humanoid *)enemies->data[j])->visible = false;
          // printf("%d ", enemies->size);
          // removeFromVector(enemies, j);
          // printf("%d ", enemies->size);
        }
      }
    }
  }

  globalTime++;
}
